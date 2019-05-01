type departureName = string;

type t = {
  actual: bool,
  blockNumber: int,
  text: string,
  time: string,
  name: departureName,
  gate: string,
  routeId: Route.routeId,
  directionName: Direction.directionName,
  terminal: string,
  vehicleHeading: int,
  vehicleLatitude: float,
  vehicleLongitude: float,
};

let toString = (d: t) =>
  Printf.sprintf(
    "<Departure actual=%b blockNumber=%d text=%s time=%s name=%s gate=%s routeId=%s directionName=%s terminal=%s vehicleHeading=%d vehicleLatitude=%f vehicleLongitude=%f",
    d.actual,
    d.blockNumber,
    d.text,
    d.time,
    d.name,
    d.gate,
    d.routeId,
    d.directionName,
    d.terminal,
    d.vehicleHeading,
    d.vehicleLatitude,
    d.vehicleLongitude,
  );

let departureActualKey = "Actual";

let departureBlockNumberKey = "BlockNumber";

let departureTextKey = "DepartureText";

let departureTimeKey = "DepartureTime";

let departureNameKey = "Description";

let departureGateKey = "Gate";

let departureRouteIdKey = "Route";

let departureDirectionNameKey = "RouteDirection";

let departureTerminalKey = "Terminal";

let departureVehicleHeadingKey = "VehicleHeading";

let departureVehicleLatitudeKey = "VehicleLatitude";

let departureVehicleLongitudeKey = "VehicleLongitude";

let df = Json.Decode.float;

let decodeDeparture = str =>
  Json.Decode.{
    actual: str |> field(departureActualKey, bool),
    blockNumber: str |> field(departureBlockNumberKey, int),
    text: str |> field(departureTextKey, string),
    time: str |> field(departureTimeKey, string),
    name: str |> field(departureNameKey, string),
    gate: str |> field(departureGateKey, string),
    routeId: str |> field(departureRouteIdKey, string),
    directionName: str |> field(departureDirectionNameKey, string),
    terminal: str |> field(departureTerminalKey, string),
    vehicleHeading: str |> field(departureVehicleHeadingKey, int),
    vehicleLatitude: str |> field(departureVehicleLatitudeKey, df),
    vehicleLongitude: str |> field(departureVehicleLongitudeKey, df),
  };

let ef = Json.Encode.float;

let encodeDeparture = d =>
  Json.Encode.(
    object_([
      (departureActualKey, bool(d.actual)),
      (departureBlockNumberKey, int(d.blockNumber)),
      (departureTextKey, string(d.text)),
      (departureTimeKey, string(d.time)),
      (departureNameKey, string(d.name)),
      (departureGateKey, string(d.gate)),
      (departureRouteIdKey, string(d.routeId)),
      (departureDirectionNameKey, string(d.directionName)),
      (departureTerminalKey, string(d.terminal)),
      (departureVehicleHeadingKey, int(d.vehicleHeading)),
      (departureVehicleLatitudeKey, ef(d.vehicleLatitude)),
      (departureVehicleLongitudeKey, ef(d.vehicleLongitude)),
    ])
  );

let ofJson = str => Util.parseJsonList(str, decodeDeparture);

let toJson = dList => Util.encodeJsonList(dList, encodeDeparture);

let timeRe = [%re {|/Date\((\d+)([-+]\d+)\)/|}];

let timeZoneOffset = Js.Date.(make() |> getTimezoneOffset) *. 60.0 *. 1000.0;

let parseTime = (~offset=0.0, time) =>
  switch (Js.String.match(timeRe, time)) {
  | Some(ar) =>
    if (Array.length(ar) == 3) {
      let millis = float_of_string(ar[1]);
      let localOffsetHours = float_of_string(ar[2]) /. 100.0;
      let localOffsetMillis = localOffsetHours *. 3600.0 *. 1000.0;
      Some(Js.Date.fromFloat(millis +. localOffsetMillis +. offset));
    } else {
      None;
    }
  | None => None
  };

let parseLocalTime = time => parseTime(~offset=timeZoneOffset, time);

let id = d =>
  Printf.sprintf("%s-%s-%s-%b-%s", d.name, d.routeId, d.directionName, d.actual, d.time)
  |> Digest.string
  |> Digest.to_hex;

let displayName = d => {
  let o = s => {
    let v = String.trim(s);
    if (v != "") {
      Some(v);
    } else {
      None;
    };
  };
  let qualifier =
    switch (o(d.gate), o(d.terminal)) {
    | (Some(gate), _) => gate
    | (None, Some(terminal)) => terminal
    | (None, None) => ""
    };
  Printf.sprintf("%s%s: %s", d.routeId, qualifier, d.name);
};
