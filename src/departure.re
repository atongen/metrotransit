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
    vehicleLatitude: str |> field(departureVehicleLatitudeKey, float),
    vehicleLongitude: str |> field(departureVehicleLongitudeKey, float),
  };

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
      (departureVehicleLatitudeKey, float(d.vehicleLatitude)),
      (departureVehicleLongitudeKey, float(d.vehicleLongitude)),
    ])
  );

let ofJson = str => Util.parseJsonList(str, decodeDeparture);

let toJson = dList => Util.encodeJsonList(dList, encodeDeparture);
