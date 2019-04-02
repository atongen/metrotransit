open Belt.Result;
open Jest;
open Expect;

describe("provider", () => {
  test("json conversion", () => {
    let str = {eostr|
        [{"Text":"University of Minnesota","Value":"1"},{"Text":"Airport (MAC)","Value":"2"},{"Text":"Other","Value":"3"},{"Text":"Prior Lake","Value":"4"},{"Text":"Scott County","Value":"5"},{"Text":"Metro Transit/Met Council","Value":"6"},{"Text":"SouthWest Transit","Value":"7"},{"Text":"Metro Transit","Value":"8"},{"Text":"Minnesota Valley","Value":"9"},{"Text":"Plymouth","Value":"10"},{"Text":"Met Council","Value":"11"},{"Text":"Maple Grove","Value":"12"}]
    |eostr};
    switch (Provider.ofJson(str)) {
    | Ok(providers) =>
      let res = Provider.toJson(providers);
      expect(String.trim(str)) |> toEqual(res);
    | Error(s) => failwith(s)
    };
  });

  test("invalid json", () => {
    let str = "[}";
    switch (Provider.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("parse error: Unexpected token } in JSON at position 1")
    };
  });

  test("invalid provider", () => {
    let str = "[{\"one\":1}]";
    switch (Provider.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("decode error: Expected field 'Value' in array at index 0")
    };
  });
});
