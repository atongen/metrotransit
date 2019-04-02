NAME=metrotransit

.PHONY: default build test clean

default: build

build:
	npm run-script build

test: build
	npm run-script test

clean:
	npm run-script clean
