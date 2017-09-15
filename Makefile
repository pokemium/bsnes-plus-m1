include bsnes/nall/Makefile

.PHONY: all bsnes dist clean

default: bsnes

bsnes:
	@$(MAKE) -C bsnes -j4

all:
ifeq ($(platform),osx)
	@rm -frd ./bsnes+.app
	@rm -frd ./bsnes+accuracy.app
endif
	@$(MAKE) clean -C bsnes
	@$(MAKE) -C bsnes -j4
	@$(MAKE) clean -C bsnes
	@$(MAKE) -C bsnes -j4 profile=accuracy

dist:
ifeq ($(platform),osx)
	@./build_distribution_osx
endif

clean:
	@$(MAKE) clean -C bsnes
