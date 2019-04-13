NAME=metrotransit

.PHONY: default build webpack test clean prod all

default: all

clean:
	npm run-script clean
	rm -f public/index.html
	rm -f public/assets/*

build:
	npm run-script build

test: build
	npm run-script test

webpack:
	npm run-script webpack

prod: clean build
	npm run-script webpack:production

deploy: prod
	scp -r public/* mercury:/var/www/metrotransit/current/public/

all: build webpack
