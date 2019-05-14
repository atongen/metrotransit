open Belt.Result;

open Jest;

open Expect;

describe("config json conversion", () => {
  let route = Route.make("543", "Some Really Long String", "A1");
  let direction = Direction.make("1", "SOUTHBOUND");
  let stop = Stop.make("144", "Wowzer St and Awesome Ave");
  let maybeConfig = Config.maybeMake(Some(route), Some(direction), Some(stop));
  switch (maybeConfig) {
  | Some(config) =>
    let jsonStr = Config.toJson(config);
    switch (Config.ofJson(jsonStr)) {
    | Ok(dc) =>
      test("id", () =>
        expect(dc.id) |> toEqual(config.id)
      );
      test("name", () =>
        expect(Config.name(dc)) |> toEqual("Some Really Long String - Southbound - Wowzer St and Awesome Ave")
      );
      test("shortName", () =>
        expect(Config.shortName(dc)) |> toEqual("Some Really - South - Wowzer St")
      );
      test("route", () =>
        expect(dc.route) |> toEqual(route)
      );
      test("routes", () =>
        expect(dc.routes) |> toEqual([])
      );
      test("direction", () =>
        expect(dc.direction) |> toEqual(direction)
      );
      test("directions", () =>
        expect(dc.directions) |> toEqual([])
      );
      test("stop", () =>
        expect(dc.stop) |> toEqual(stop)
      );
      test("stops", () =>
        expect(dc.stops) |> toEqual([])
      );
    | Error(s) => failwith(s)
    };
  | None => failwith("did not get a config")
  };
});
