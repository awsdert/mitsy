%: force
	cd ./src/ && make $(MAKEFILES) $(MAKECMDGOALS)
force: ;
