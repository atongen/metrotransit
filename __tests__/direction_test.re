open Belt.Result;
open Jest;
open Expect;

describe("direction", () => {
  test("json conversion", () => {
    let str = {eostr|
        [{"Text":"SOUTHBOUND","Value":"1"},{"Text":"EASTBOUND","Value":"2"},{"Text":"WESTBOUND","Value":"3"},{"Text":"NORTHBOUND","Value":"4"}]
    |eostr};
    switch (Direction.ofJson(str)) {
    | Ok(directions) =>
      let res = Direction.toJson(directions);
      expect(String.trim(str)) |> toEqual(res);
    | Error(s) => failwith(s)
    };
  });

  test("invalid json", () => {
    let str = "[}";
    switch (Direction.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("parse error: Unexpected token } in JSON at position 1")
    };
  });

  test("invalid direction", () => {
    let str = "[{\"one\":1}]";
    switch (Direction.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("decode error: Expected field 'Value' in array at index 0")
    };
  });
});
