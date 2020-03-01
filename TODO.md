Schritte für Release:
- buildnummer hoch setzen
- geänderte Dateien in einen ordner mit der hirarchie bin/ lib/ etc, wie im DBManager RC verschieben
- diesen Ordner ohne überordner packen und aktuell DBManager.tar.gz nennen (ich überlege das update auch als solches zu kennzeichnen, vielleicht DBManager_update.tar.gz o.Ä.
- neuen eintrag in update.xml anlegen und da drin die Buildno anlegen, da drin außerdem einen eintrag file(s) mit der size als attribute und dem relativen pfad in der zip als inhalt:
    <Update>
      <Application>
          <BuildNo>
              0.0.9
          </BuildNo>
      </Application>
      <Files>
          <File size="497082">
              <![CDATA[DBManager.tar.gz]]>
          </File>
      </Files>
    </Update>
# erstellen des RC an sich
- alle notwendigen dateien in einen ordner verschieben (auch lib, bin, sqlitedriver, platform, etc)
- diesen ordner als DBManager.tar.gz packen
- DBManager.tar.gz in public online stellen (wenn die beiden pakete namentlich getrennt sind, könnte man auch beide in public/DBManager ablegen
- DBManager.tar.gz update file in public/DBManager legen
- update.xml in public/DBManager legen
