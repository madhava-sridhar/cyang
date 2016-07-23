all: leg
	./peg-0.1.16/leg yang.leg -o yang.leg.c
	gcc  yang.leg.c -o cyang

leg:
	$(MAKE) -C peg-0.1.16/

test:
	for i in tests/*.yang; do echo ">> Parsing $$i"; cat $$i | ./cyang; done
clean:
	rm -f yang.leg.c cyang

distclean: clean
	$(MAKE) -C peg-0.1.16/ clean
