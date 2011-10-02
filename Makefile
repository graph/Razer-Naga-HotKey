
.PHONEY: install all
all: nagad

install: nagad
	cp naga.rules /etc/udev/rules.d/naga.rules
	cp nagad /usr/local/bin/nagad
	cp naga_plugged.pl /usr/local/bin/
	cp naga_disablekbd.pl /usr/local/bin
	chmod +x /usr/local/bin/naga_plugged.pl
	chmod +x /usr/local/bin/naga_disablekbd.pl

uninstall:
	rm -f /etc/udev/rules.d/naga.rules
	rm -f /usr/local/bin/naged
	rm -f /usr/local/bin/naga_plugged.pl
	rm -f /usr/local/bin/naga_disablekbd.pl

clean:
	rm -f nagad
