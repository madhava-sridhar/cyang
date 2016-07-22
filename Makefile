all:
	$(MAKE) -C peg-0.1.16/
	./peg-0.1.16/leg yang.leg -o yang.leg.c
	gcc  yang.leg.c -o cyang

test:
	for i in tests/*.yang; do echo Parsing $$i; cat $$i | ./cyang; done
clean:
	rm -f yang.leg.c cyang
