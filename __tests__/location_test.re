open Belt.Result;
open Jest;
open Expect;

describe("location", () => {
  test("json conversion", () => {
    let str = {eostr|
        [{"Bearing":45,"BlockNumber":1115,"Direction":1,"LocationTime":"/Date(1554235805000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"E","VehicleLatitude":44.85319,"VehicleLongitude":-93.23816},{"Bearing":90,"BlockNumber":1120,"Direction":1,"LocationTime":"/Date(1554235803000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"A","VehicleLatitude":44.99192,"VehicleLongitude":-93.29512},{"Bearing":270,"BlockNumber":1123,"Direction":4,"LocationTime":"/Date(1554235802000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"M","VehicleLatitude":45.03553,"VehicleLongitude":-93.29691},{"Bearing":45,"BlockNumber":1128,"Direction":4,"LocationTime":"/Date(1554235804000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"M","VehicleLatitude":45.06051,"VehicleLongitude":-93.31948},{"Bearing":315,"BlockNumber":1130,"Direction":1,"LocationTime":"/Date(1554235802000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"B","VehicleLatitude":44.9339,"VehicleLongitude":-93.26302},{"Bearing":180,"BlockNumber":1133,"Direction":1,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":8.9408,"Terminal":"E","VehicleLatitude":44.86469,"VehicleLongitude":-93.26819},{"Bearing":0,"BlockNumber":1140,"Direction":4,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":6.7056,"Terminal":"M","VehicleLatitude":44.94517,"VehicleLongitude":-93.26261},{"Bearing":180,"BlockNumber":1141,"Direction":4,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":26.8224,"Terminal":"M","VehicleLatitude":45.0586,"VehicleLongitude":-93.28733},{"Bearing":270,"BlockNumber":1142,"Direction":4,"LocationTime":"/Date(1554235804000-0500)/","Odometer":0,"Route":"5","Speed":0.44704,"Terminal":"F","VehicleLatitude":44.86019,"VehicleLongitude":-93.24213},{"Bearing":0,"BlockNumber":1143,"Direction":4,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"F","VehicleLatitude":44.99946,"VehicleLongitude":-93.29408},{"Bearing":90,"BlockNumber":1145,"Direction":1,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"A","VehicleLatitude":45.03543,"VehicleLongitude":-93.29592},{"Bearing":180,"BlockNumber":1146,"Direction":1,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"B","VehicleLatitude":44.97693,"VehicleLongitude":-93.26865},{"Bearing":0,"BlockNumber":1147,"Direction":4,"LocationTime":"/Date(1554235807000-0500)/","Odometer":0,"Route":"5","Speed":4.4704,"Terminal":"M","VehicleLatitude":45.01511,"VehicleLongitude":-93.29557},{"Bearing":315,"BlockNumber":1148,"Direction":4,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":6.7056,"Terminal":"M","VehicleLatitude":44.97322,"VehicleLongitude":-93.2635},{"Bearing":135,"BlockNumber":1149,"Direction":1,"LocationTime":"/Date(1554235803000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"E","VehicleLatitude":44.97573,"VehicleLongitude":-93.26768},{"Bearing":270,"BlockNumber":1150,"Direction":1,"LocationTime":"/Date(1554235803000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"A","VehicleLatitude":45.05948,"VehicleLongitude":-93.31867},{"Bearing":180,"BlockNumber":1151,"Direction":1,"LocationTime":"/Date(1554235803000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"E","VehicleLatitude":44.92522,"VehicleLongitude":-93.2626},{"Bearing":315,"BlockNumber":1153,"Direction":4,"LocationTime":"/Date(1554235807000-0500)/","Odometer":0,"Route":"5","Speed":6.7056,"Terminal":"M","VehicleLatitude":44.97927,"VehicleLongitude":-93.27731},{"Bearing":0,"BlockNumber":1154,"Direction":4,"LocationTime":"/Date(1554235808000-0500)/","Odometer":0,"Route":"5","Speed":11.176,"Terminal":"L","VehicleLatitude":44.91406,"VehicleLongitude":-93.2626},{"Bearing":135,"BlockNumber":1155,"Direction":1,"LocationTime":"/Date(1554235805000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"E","VehicleLatitude":44.97971,"VehicleLongitude":-93.27499},{"Bearing":0,"BlockNumber":1157,"Direction":1,"LocationTime":"/Date(1554235807000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"E","VehicleLatitude":45.00589,"VehicleLongitude":-93.30943},{"Bearing":90,"BlockNumber":1158,"Direction":4,"LocationTime":"/Date(1554235806000-0500)/","Odometer":0,"Route":"5","Speed":26.8224,"Terminal":"M","VehicleLatitude":44.89007,"VehicleLongitude":-93.28315},{"Bearing":180,"BlockNumber":1310,"Direction":1,"LocationTime":"/Date(1554235802000-0500)/","Odometer":0,"Route":"5","Speed":8.9408,"Terminal":"B","VehicleLatitude":44.96373,"VehicleLongitude":-93.26258},{"Bearing":180,"BlockNumber":1462,"Direction":1,"LocationTime":"/Date(1554235806000-0500)/","Odometer":0,"Route":"5","Speed":8.9408,"Terminal":"E","VehicleLatitude":44.86031,"VehicleLongitude":-93.26818},{"Bearing":315,"BlockNumber":1479,"Direction":4,"LocationTime":"/Date(1554235802000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"M","VehicleLatitude":44.97712,"VehicleLongitude":-93.27297},{"Bearing":0,"BlockNumber":1566,"Direction":4,"LocationTime":"/Date(1554235805000-0500)/","Odometer":0,"Route":"5","Speed":0,"Terminal":"F","VehicleLatitude":44.9554,"VehicleLongitude":-93.2625}]
    |eostr};
    switch (Location.ofJson(str)) {
    | Ok(locations) =>
      let res = Location.toJson(locations);
      expect(String.trim(str)) |> toEqual(res);
    | Error(s) => failwith(s)
    };
  });

  test("invalid json", () => {
    let str = "[}";
    switch (Location.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("parse error: Unexpected token } in JSON at position 1")
    };
  });

  test("invalid location", () => {
    let str = "[{\"one\":1}]";
    switch (Location.ofJson(str)) {
    | Ok(_) => failwith("no error")
    | Error(s) => expect(s) |> toEqual("decode error: Expected field 'Bearing' in array at index 0")
    };
  });
});
