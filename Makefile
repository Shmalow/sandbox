clean:
	list=`find . | grep "Makefile$$" | grep -v "^./Makefile$$" | perl -pe "s#/Makefile##g"`; \
	for dir in $$list; \
	do \
		cd $$dir; \
		make clean; \
		cd -; \
	done
	find . -name 'Edit.*' | xargs rm -f || :
