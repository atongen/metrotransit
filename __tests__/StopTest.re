open Belt.Result;
open Jest;
open Expect;

describe("stop", () => {
  test("json conversion", () => {
    let str = {eostr|
        [{"Text":"Mall of America Transit Station","Value":"MAAM"},{"Text":"Portland Ave and 77th St","Value":"77PO"},{"Text":"Portland Ave and 66th St","Value":"66PO"},{"Text":"Chicago Ave and 56th St","Value":"56CH"},{"Text":"Chicago Ave and 46th St","Value":"46CH"},{"Text":"Chicago Ave and 38th St","Value":"38CH"},{"Text":"Chicago Lake Transit Center","Value":"CHLA"},{"Text":"Chicago Ave and 8th St ","Value":"8SCH"},{"Text":"7th St  and Nicollet Mall","Value":"7SNI"},{"Text":"7th St  and Olson Memorial Hwy","Value":"7SOL"},{"Text":"Emerson Ave  and W Broadway Ave","Value":"EMBD"},{"Text":"26th Ave and Penn Ave","Value":"26PN"},{"Text":"33rd Ave  and Fremont Ave ","Value":"33FM"},{"Text":"44th Ave  and Fremont Ave ","Value":"44FM"},{"Text":"Osseo Rd and 47th Ave ","Value":"47OS"},{"Text":"Brooklyn Center Transit Center","Value":"BCTC"}]
    |eostr};
    switch (Stop.ofJson(str)) {
    | Ok(stops) =>
      let res = Stop.toJson(stops);
      expect(String.trim(str)) |> toEqual(res);
    | Error(s) => failwith(s)
    };
  });

  test("invalid json", () => {
    let str = "[}";
    switch (Stop.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("parse error: Unexpected token } in JSON at position 1")
    };
  });

  test("invalid stop", () => {
    let str = "[{\"one\":1}]";
    switch (Stop.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("decode error: Expected field 'Value' in array at index 0")
    };
  });
});
