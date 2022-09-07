PFILES		=	dep2cep  exprf  unparsecpp  utils
XWAMFILES	=	$(addsuffix .xwam, $(PFILES))

%.xwam: %.P
	xsb -e "compile('$<'),halt."

all: $(XWAMFILES)

# exprf.h is a generated file, but it is committed for convenience
# Following target is only for developers' purposes
exprf.h: exprf.P  unparsecpp.P  utils.P
	echo [exprf]. | xsb > $@
