all:
# simple twotrack

jar:
	cd .. && make JavaRave.jar

simple: jar $(srcdir)/Simple.java
	$(JAVAC) -cp ../JavaRave.jar $(srcdir)/Simple.java -d .
	$(JAVA) -cp ../JavaRave.jar:. Simple

twotrack: jar $(srcdir)/TwoTrack.java
	$(JAVAC) -cp ../JavaRave.jar $(srcdir)/TwoTrack.java -d .
	$(JAVA) -cp ../JavaRave.jar:. TwoTrack

flavor: jar $(srcdir)/Flavor.java
	$(JAVAC) -cp ../JavaRave.jar $(srcdir)/Flavor.java -d .
	$(JAVA) -cp ../JavaRave.jar:. Flavor

CLEANFILES = Simple.class TwoTrack.class

EXTRA_DIST = Simple.java TwoTrack.java
