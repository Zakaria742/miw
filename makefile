start:
	@echo "run the make install/remove with sudo"
install:
	clear
	gcc -o miw src/miw.c -lm
	mv ./miw /usr/bin/miw
	chmod 755 /usr/bin/miw
	cp -p ./miw.1 /usr/share/man/man1
remove:
	rm -rf /usr/bin/miw
	rm -rf /usr/share/man/man1/miw.*
