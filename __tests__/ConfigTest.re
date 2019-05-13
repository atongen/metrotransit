open Belt.Result;

open Jest;

open Expect;

describe("config json conversion", () => {
    let route = Route.make("5", "Fiver", "ok");
    let direction = Direction.make("1", "SOUTHBOUND");
    let stop = Stop.make("144", "ABCD");
    let maybeConfig = Config.maybeMake(Some(route), Some(direction), Some(stop));
    switch (maybeConfig) {
    | Some(config) =>
      let jsonStr = Config.toJson(config);
      switch (Config.ofJson(jsonStr)) {
      | Ok(dc) =>
        test("id", () => {
            expect(dc.id) |> toEqual(config.id);
        });
        test("name", () => {
            expect(dc.name) |> toEqual(config.name);
        });
        test("shortName", () => {
            expect(dc.shortName) |> toEqual(config.shortName);
        });
        test("routeId", () => {
            expect(dc.routeId) |> toEqual(config.routeId);
        });
        test("route", () => {
            expect(dc.route) |> toEqual(None);
        });
        test("routes", () => {
            expect(dc.routes) |> toEqual([]);
        });
        test("directionId", () => {
            expect(dc.directionId) |> toEqual(config.directionId);
        });
        test("direction", () => {
            expect(dc.direction) |> toEqual(None);
        });
        test("directions", () => {
            expect(dc.directions) |> toEqual([]);
        });
        test("stopId", () => {
            expect(dc.stopId) |> toEqual(config.stopId);
        });
        test("stop", () => {
            expect(dc.stop) |> toEqual(None);
        });
        test("stops", () => {
            expect(dc.stops) |> toEqual([]);
        });
      | Error(s) => failwith(s)
      };
    | None => failwith("did not get a config")
    };
});
