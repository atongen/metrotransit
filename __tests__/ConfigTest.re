open Belt.Result;

open Jest;

open Expect;

describe("config", () =>
  test("json conversion", () => {
    let route = Route.make("5", "Fiver", "ok");
    let direction = Direction.make("1", "SOUTHBOUND");
    let stop = Stop.make("144", "ABCD");
    let maybeConfig = Config.maybeMake(Some(route), Some(direction), Some(stop));
    switch (maybeConfig) {
    | Some(config) =>
      let jsonStr = Config.toJson(config);
      switch (Config.ofJson(jsonStr)) {
      | Ok(decodedConfig) => expect(decodedConfig) |> toEqual(config)
      | Error(s) => failwith(s)
      };
    | None => failwith("did not get a config")
    };
  })
);
