CPPCHECK = cppcheck
SOURCES = $(wildcard *.cpp tests/*.cpp)

.PHONY: lint
lint:
	$(CPPCHECK) --enable=warning,performance,portability --error-exitcode=1 --std=c++17 --suppress=missingIncludeSystem $(SOURCES)
