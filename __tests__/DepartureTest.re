open Belt.Result;

open Jest;

open Expect;

describe("departure", () => {
  test("json conversion", () => {
    let str = {eostr|
        [{"Actual":true,"BlockNumber":1150,"DepartureText":"10 Min","DepartureTime":"/Date(1554233640000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.97282,"VehicleLongitude":-93.26239},{"Actual":true,"BlockNumber":1144,"DepartureText":"12 Min","DepartureTime":"/Date(1554233760000-0500)/","Description":"Emerson/26Av-Broadway","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"F","VehicleHeading":0,"VehicleLatitude":44.97347,"VehicleLongitude":-93.2639},{"Actual":true,"BlockNumber":1141,"DepartureText":"12 Min","DepartureTime":"/Date(1554233760000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.9711,"VehicleLongitude":-93.26223},{"Actual":true,"BlockNumber":1128,"DepartureText":"19 Min","DepartureTime":"/Date(1554234180000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.96252,"VehicleLongitude":-93.26245},{"Actual":false,"BlockNumber":1123,"DepartureText":"2:50","DepartureTime":"/Date(1554234600000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.94967,"VehicleLongitude":-93.26256},{"Actual":false,"BlockNumber":1147,"DepartureText":"2:57","DepartureTime":"/Date(1554235020000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.93763,"VehicleLongitude":-93.26242},{"Actual":false,"BlockNumber":1143,"DepartureText":"3:04","DepartureTime":"/Date(1554235440000-0500)/","Description":"Emerson/26Av-Broadway","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"F","VehicleHeading":0,"VehicleLatitude":44.91126,"VehicleLongitude":-93.26264},{"Actual":false,"BlockNumber":1153,"DepartureText":"3:11","DepartureTime":"/Date(1554235860000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":0,"VehicleLongitude":0},{"Actual":false,"BlockNumber":1479,"DepartureText":"3:18","DepartureTime":"/Date(1554236280000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.86003,"VehicleLongitude":-93.2489},{"Actual":false,"BlockNumber":1148,"DepartureText":"3:25","DepartureTime":"/Date(1554236700000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.90496,"VehicleLongitude":-93.26268},{"Actual":false,"BlockNumber":1566,"DepartureText":"3:34","DepartureTime":"/Date(1554237240000-0500)/","Description":"Emerson/26Av-Broadway","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"F","VehicleHeading":0,"VehicleLatitude":44.86257,"VehicleLongitude":-93.23778},{"Actual":false,"BlockNumber":1140,"DepartureText":"3:40","DepartureTime":"/Date(1554237600000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.86071,"VehicleLongitude":-93.26817},{"Actual":false,"BlockNumber":1154,"DepartureText":"3:49","DepartureTime":"/Date(1554238140000-0500)/","Description":"Emerson / To 33 Av","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"L","VehicleHeading":0,"VehicleLatitude":0,"VehicleLongitude":0},{"Actual":false,"BlockNumber":1130,"DepartureText":"3:55","DepartureTime":"/Date(1554238500000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.96612,"VehicleLongitude":-93.26263},{"Actual":false,"BlockNumber":1142,"DepartureText":"4:04","DepartureTime":"/Date(1554239040000-0500)/","Description":"Emerson/26Av-Broadway","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"F","VehicleHeading":0,"VehicleLatitude":44.97017,"VehicleLongitude":-93.31578},{"Actual":false,"BlockNumber":1158,"DepartureText":"4:10","DepartureTime":"/Date(1554239400000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":0,"VehicleLongitude":0},{"Actual":false,"BlockNumber":1115,"DepartureText":"4:18","DepartureTime":"/Date(1554239880000-0500)/","Description":"Emerson/26Av-Broadway","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"F","VehicleHeading":0,"VehicleLatitude":44.9419,"VehicleLongitude":-93.26262},{"Actual":false,"BlockNumber":1217,"DepartureText":"4:24","DepartureTime":"/Date(1554240240000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":0,"VehicleLongitude":0},{"Actual":false,"BlockNumber":1163,"DepartureText":"4:31","DepartureTime":"/Date(1554240660000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":0,"VehicleLongitude":0},{"Actual":false,"BlockNumber":1120,"DepartureText":"4:39","DepartureTime":"/Date(1554241140000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":45.05939,"VehicleLongitude":-93.31875},{"Actual":false,"BlockNumber":1133,"DepartureText":"4:50","DepartureTime":"/Date(1554241800000-0500)/","Description":"Emerson/26Av-Broadway","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"F","VehicleHeading":0,"VehicleLatitude":44.97921,"VehicleLongitude":-93.2778},{"Actual":false,"BlockNumber":1167,"DepartureText":"4:56","DepartureTime":"/Date(1554242160000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":0,"VehicleLongitude":0},{"Actual":false,"BlockNumber":1151,"DepartureText":"5:03","DepartureTime":"/Date(1554242580000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":45.00495,"VehicleLongitude":-93.29565},{"Actual":false,"BlockNumber":1150,"DepartureText":"5:11","DepartureTime":"/Date(1554243060000-0500)/","Description":"Fremont Av/Brklyn Ctr/Transit Ctr","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"M","VehicleHeading":0,"VehicleLatitude":44.97282,"VehicleLongitude":-93.26239},{"Actual":false,"BlockNumber":1149,"DepartureText":"5:20","DepartureTime":"/Date(1554243600000-0500)/","Description":"Emerson/26Av-Broadway","Gate":"","Route":"5","RouteDirection":"NORTHBOUND","Terminal":"F","VehicleHeading":0,"VehicleLatitude":44.99942,"VehicleLongitude":-93.29407}]
    |eostr};
    switch (Departure.ofJson(str)) {
    | Ok(departures) =>
      let res = Departure.toJson(departures);
      expect(String.trim(str)) |> toEqual(res);
    | Error(s) => failwith(s)
    };
  });
  test("invalid json", () => {
    let str = "[}";
    switch (Departure.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("parse error: Unexpected token } in JSON at position 1")
    };
  });
  test("invalid departure", () => {
    let str = "[{\"one\":1}]";
    switch (Departure.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("decode error: Expected field 'Actual' in array at index 0")
    };
  });
  /* 2019-04-02T19:34:00.000Z (utc) */
  let time = "/Date(1554233640000-0500)/";
  switch (Departure.parseLocalTime(time)) {
  | Some(date) =>
    test("parseTime year", () =>
      expect(Js.Date.getFullYear(date)) |> toEqual(2019.0)
    );
    test("parseTime month", () =>
      expect(Js.Date.getMonth(date)) |> toEqual(3.0)
    );
    test("parseTime date", () =>
      expect(Js.Date.getDate(date)) |> toEqual(2.0)
    );
    test("parseTime hours", () =>
      expect(Js.Date.getHours(date)) |> toEqual(14.0)
    );
    test("parseTime minutes", () =>
      expect(Js.Date.getMinutes(date)) |> toEqual(34.0)
    );
    test("parseTime seconds", () =>
      expect(Js.Date.getSeconds(date)) |> toEqual(0.0)
    );
  | None => failwith("Did not parse departure time")
  };
});
