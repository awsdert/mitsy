MAKECMDGOALS?=all
$(MAKECMDGOALS):
	cd ./src/ && make --no-print-directory $(MAKEFILES) $(MAKECMDGOALS)
