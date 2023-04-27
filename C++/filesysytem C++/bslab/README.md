# Labor Betriebssysteme

Dies ist das Template für die Aufgaben zum Labor Betriebsysteme. Details zur Aufgabenstellung finden Sie im ILIAS-Kurs 
zum Labor.

Wenn die notwendige Arbeitsumgebung eingerichtet wurde, sollte sich das Template-Projekt korrekt übersetzen lassen und 
dann die Funktionalität des 
[_Simple & Stupid File Systems_](http://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/) 
bereitstellen. Das lässt sich mit den folgenden Kommandos ausprobieren (dabei 
`<pfad-zum-dateisystem>` durch das Verzeichnis mit dem Template ersetzen):

	mkdir build
	cd build
	cmake <pfad-zum-template> && make
	mkdir mount
	bin/mount.myfs mount -l log.txt
	cd mount
	ls
	cat file349
	cat file54
	cd ..
	fusermount -u mount # oder für Mac OS-X: umount mount
	

Folgende Informationen können noch hilfreich sein:

* [Anlegen eines GIT-Projekts für Ihr Team](documentation/createGitProject.md)
* [Importieren des Projekts in Eclipse](documentation/eclipseImportProject.md)
* [Konfiguration von älteren Eclipse-Versionen](documentation/eclipseConfig.md)
* [Einrichten der Arbeitsumgebung unter Linux](documentation/setupLinux.md)
* [Einrichten der Arbeitsumgebung unter MacOS](documentation/setupMacos.md)
* [Entwickeln mit Docker](documentation/setupDocker.md) (z.B. unter Microsoft Windows)

Git zum funktionieren bringen:
- Klonen mit ```git clone https://github.com/lubp64357/bslab.git```
- [auth token erstellen] (https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token), man muss glaube nur repo aktivieren, den token muss man auch irgendwo sichern oder [das](https://www.shellhacks.com/git-config-username-password-store-credentials/) aktivieren
- Im geklonten Ordner dann ```git remote remove origin```
- ```git remote add origin https://github_name@github.com/lubp64357/bslab.git```
- ```git pull origin``` und then auth token da reinkopieren

Falls man 2 github accounts hat muss man noch:
git config user.name "tolle.mail@gmail.com"
git config user.email "tolle.mail@gmail.com"