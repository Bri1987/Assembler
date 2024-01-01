build:
	rm -rf cmake-build-debug
	mkdir cmake-build-debug
	cd cmake-build-debug && cmake ..
	make -C cmake-build-debug

test:
	chmod 777 ./test/sy.sh
	@printf '\e[32mBegin Test\e[0m\n'
	./test/sy.sh
	@printf '\e[32mPassed all tests\e[0m\n'

clean:
	rm -rf test/out/
	rm -rf ./cmake-build-debug/as_final

.PHONY: build clean test