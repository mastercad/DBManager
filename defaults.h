#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <QString>

namespace Defaults {
    namespace MYSQL {
        const QString HOST = "localhost";
        const int PORT = 3306;
        const QString USER = "root";
        const QString PASSWORD = "root";
    }

    class Resolver {
        public:
        static QString resolve(QString key, QString currentValue);
        static int resolve(QString key, int currentValue);
    };
}

namespace Application {
    const QString BuildNo = "0.0.9";
    const QString UpdateHost = "byte-artist.de";
    const QString UpdateFile = "update.xml";
    const QString UpdatePath = "/DBManager";
    const QString UpdateDonwloadUrl = "https://byte-artist.de/DBManager/";
    const QString UpdateUrl = "https://byte-artist.de/DBManager/update.xml";
}

#endif // DEFAULTS_H
