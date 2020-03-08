/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "textedit.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

#include <QDebug>

TextEdit::TextEdit(Connection* connection, QWidget *parent)
    : QTextEdit(parent),
      connection(connection)
{
}

TextEdit::~TextEdit() {
}

void TextEdit::setConnection(Connection* connection) {
    this->connection = connection;
}

Connection* TextEdit::getConnection() {
    return this->connection;
}

void TextEdit::insertCompletion(const QString &completion) {

    if (completer->widget() != this) {
        return;
    }

    QTextCursor tc = textCursor();
    int extra = completion.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString TextEdit::textNearCursor(int movePosition = 0) const {
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString textUnderCursor = tc.selectedText();

    QTextCursor textCursorLeft = textCursor();
    textCursorLeft.movePosition(QTextCursor::WordLeft);
    textCursorLeft.select(QTextCursor::WordUnderCursor);
    QString textLeftCursor = textCursorLeft.selectedText();

    QTextCursor textCursorLeftLeft = textCursor();
    textCursorLeftLeft.movePosition(QTextCursor::WordLeft);
    textCursorLeftLeft.movePosition(QTextCursor::WordLeft);
    textCursorLeftLeft.select(QTextCursor::WordUnderCursor);
    QString textLeftLeftCursor = textCursorLeftLeft.selectedText();

    qDebug() << "Text under Cursor: " << textUnderCursor;
    qDebug() << "Text left Cursor: " << textLeftCursor;
    qDebug() << "Text left left Cursor: " << textLeftLeftCursor;

    return textCursorLeft.selectedText();
}

void TextEdit::focusInEvent(QFocusEvent *event) {
    if (completer) {
        completer->setWidget(this);
    }
    QTextEdit::focusInEvent(event);
}

void TextEdit::ensureCompleterExists() {
    if (nullptr == completer) {
        completer = new QCompleter();
        QObject::connect(completer, QOverload<const QString &>::of(&QCompleter::activated), this, &TextEdit::insertCompletion);
    }

    if (nullptr != connection
        && nullptr != connection->getDatabaseCollection()
    ) {
        completer->setModel(connection->getDatabaseCollection());
        completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    }
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    completer->setWidget(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
}

void TextEdit::keyPressEvent(QKeyEvent *event) {
    ensureCompleterExists();

    // The following keys are forwarded by the completer to the widget
    switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            event->ignore();
            return; // let the completer do default behavior
        default:
            break;
    }

    const bool isShortcut = (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_Space); // CTRL+Space

    if (!isShortcut) {
        qDebug() << "Taste ist kein shortcur für completion!";
        QTextEdit::keyPressEvent(event);
        return;
    }

    qDebug() << "Shortcut für completion erkannt!";
    const bool ctrlOrShift = event->modifiers().testFlag(Qt::ControlModifier) || event->modifiers().testFlag(Qt::ShiftModifier);

    if (ctrlOrShift
        && event->text().isEmpty()
    ) {
        return;
    }

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textNearCursor();

    if ("." == completionPrefix) {
        QTextCursor textCursorLeft = textCursor();
        textCursorLeft.movePosition(QTextCursor::WordLeft, QTextCursor::MoveAnchor, 2);
        textCursorLeft.select(QTextCursor::WordUnderCursor);

        completionPrefix = textCursorLeft.selectedText();
    }

    if (
        !isShortcut
        && (hasModifier
            || event->text().isEmpty()
            || 3 > completionPrefix.length()
            || eow.contains(event->text().right(1))
        )
    ) {
        completer->popup()->hide();
        return;
    }

    if (completionPrefix != completer->completionPrefix()) {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
    }

    QRect curRect = cursorRect();
    curRect.setWidth(completer->popup()->sizeHintForColumn(0) + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(curRect);
}
