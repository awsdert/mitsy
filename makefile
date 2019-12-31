%: force
	cd ./src/ && make --no-print-directory $(MAKEFILES) $(MAKECMDGOALS)
force: ;
