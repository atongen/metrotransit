# Nextrip - Metrotransit

Demo: https://metrotransit.andrewtongen.net/

## Why does this exist?

https://www.metrotransit.org/NexTripBadge.aspx is great but lacks a few things that I wanted:

* Memory of previous selections - I usually select the same routes, or variations of the same routes and I wanted something that made it a bit easier to do this.
* Linkability - I wanted the ability to bookmark a link directly to a commonly viewed route.
* Code - Metrotransit offers [an api](https://svc.metrotransit.org/) and I wanted an excuse to write more [Reason React](https://reasonml.github.io/reason-react/) code!

I'm sure there are many ways to satisfy these conditions, but this is the one I chose!

Note: This project is unofficial and not associated with the [Metrotransit Organization](https://www.metrotransit.org/) in any way (other than it leverages their official api).

## Building

```
git clone https://github.com/atongen/metrotransit.git
cd metrostransit
npm install
make test
make
```

Built assets will now be present in `./public`.

## Contributing

Suggestions, bug reports and PR's welcome!
