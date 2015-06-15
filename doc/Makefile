all: html

pics:
	dia -e factory.eps factory.dia
	dia -e factory.png factory.dia

man:
	docbook-to-man rave.sgml > rave.1

html:
	docbook2html -o html/ rave.sgml

edit:
	vim rave.sgml

show:
	man -l rave.1

upload:
#	cp -a html/* ~/afs/www/rave/
#	cp -a *.jpg *.png ~/afs/www/rave/
	scp html/* stop.itp.tuwien.ac.at:/var/www/stop/docs/rave/
	scp *.jpg *.png stop.itp.tuwien.ac.at:/var/www/stop/docs/rave/


clean:
	rm -rf html
