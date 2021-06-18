VALGRIND = valgrind --tool=memcheck --leak-check=full --track-origins=yes

P1_TESTS = simple unknown_instr
P2_TESTS = labels
FULL_TESTS = labels simple1 p1_errors p2_errors

.PHONY: clean run_tests

all: test

clean:
	rm -rf out

test: ../assembler make_out_dirs run_tests
	@python3 test.py

grade: ../assembler make_out_dirs run_tests
	@python3 test.py --grade

make_out_dirs:
	@-mkdir -p out/p1
	@-mkdir -p out/p2
	@-mkdir -p out/full

run_tests: run_p1_tests run_p2_tests run_full_tests

run_p1_tests: $(addsuffix _p1_test, $(P1_TESTS))

%_p1_test: in/p1/%.s
	-$(VALGRIND) ../assembler -p1 $< out/p1/$*.int out/p1/$*.symtbl -log out/p1/$*.log 2> out/p1/$*.memcheck || true
	@diff -uZ out/p1/$*.log ref/p1/$*.log && \
	if grep "Assembly operation completed successp1y!" ref/p1/$*.log > /dev/null; then \
		diff -uZ out/p1/$*.int ref/p1/$*.int \
		&& diff -uZ out/p1/$*.symtbl ref/p1/$*.symtbl; \
	fi && echo -n "1" > out/p1/$*.result || echo -n "0" > out/p1/$*.result
	@grep "ERROR SUMMARY: 0 errors from 0 contexts" out/p1/$*.memcheck > /dev/null && echo -n " 1" >> out/p1/$*.result || echo -n " 0" >> out/p1/$*.result

run_p2_tests: $(addsuffix _p2_test, $(P2_TESTS))

%_p2_test: in/p2/%.int in/p2/%.symtbl
	-$(VALGRIND) ../assembler -p2 $^ out/p2/$*.out -log out/p2/$*.log 2> out/p2/$*.memcheck || true
	@diff -uZ out/p2/$*.log ref/p2/$*.log && \
	if grep "Assembly operation completed successfully!" ref/p2/$*.log > /dev/null; then \
		diff -uZ out/p2/$*.out ref/p2/$*.out; \
	fi && echo -n "1" > out/p2/$*.result || echo -n "0" > out/p2/$*.result
	@grep "ERROR SUMMARY: 0 errors from 0 contexts" out/p2/$*.memcheck > /dev/null && echo -n " 1" >> out/p2/$*.result || echo -n " 0" >> out/p2/$*.result

run_full_tests: $(addsuffix _full_test, $(FULL_TESTS))

%_full_test: in/full/%.s
	-$(VALGRIND) ../assembler $< out/full/$*.int out/full/$*.symtbl out/full/$*.out -log out/full/$*.log 2> out/full/$*.memcheck || true
	@diff -uZ out/full/$*.log ref/full/$*.log && \
	if grep "Assembly operation completed successfully!" ref/full/$*.log > /dev/null; then \
		diff -uZ out/full/$*.int ref/full/$*.int \
		&& diff -uZ out/full/$*.symtbl ref/full/$*.symtbl \
		&& diff -uZ out/full/$*.out ref/full/$*.out; \
	fi && echo -n "1" > out/full/$*.result || echo -n "0" > out/full/$*.result
	@grep "ERROR SUMMARY: 0 errors from 0 contexts" out/full/$*.memcheck > /dev/null && echo -n " 1" >> out/full/$*.result || echo -n " 0" >> out/full/$*.result
