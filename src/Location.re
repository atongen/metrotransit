type t = {
  bearing: int,
  blockNumber: int,
  directionId: Direction.directionId,
  time: string,
  odometer: int,
  routeId: Route.routeId,
  speed: float,
  terminal: string,
  vehicleLatitude: float,
  vehicleLongitude: float,
};

let locationBearingKey = "Bearing";

let locationBlockNumberKey = "BlockNumber";

let locationDirectionIdKey = "Direction";

let locationTimeKey = "LocationTime";

let locationOdometerKey = "Odometer";

let locationRouteIdKey = "Route";

let locationSpeedKey = "Speed";

let locationTerminalKey = "Terminal";

let locationVehicleLatitudeKey = "VehicleLatitude";

let locationVehicleLongitudeKey = "VehicleLongitude";

let decodeLocation = str =>
  Json.Decode.{
    bearing: str |> field(locationBearingKey, int),
    blockNumber: str |> field(locationBlockNumberKey, int),
    directionId: str |> field(locationDirectionIdKey, int) |> string_of_int,
    time: str |> field(locationTimeKey, string),
    odometer: str |> field(locationOdometerKey, int),
    routeId: str |> field(locationRouteIdKey, string),
    speed: str |> field(locationSpeedKey, float),
    terminal: str |> field(locationTerminalKey, string),
    vehicleLatitude: str |> field(locationVehicleLatitudeKey, float),
    vehicleLongitude: str |> field(locationVehicleLongitudeKey, float),
  };

let encodeLocation = l =>
  Json.Encode.(
    object_([
      (locationBearingKey, int(l.bearing)),
      (locationBlockNumberKey, int(l.blockNumber)),
      (locationDirectionIdKey, int(l.directionId |> int_of_string)),
      (locationTimeKey, string(l.time)),
      (locationOdometerKey, int(l.odometer)),
      (locationRouteIdKey, string(l.routeId)),
      (locationSpeedKey, float(l.speed)),
      (locationTerminalKey, string(l.terminal)),
      (locationVehicleLatitudeKey, float(l.vehicleLatitude)),
      (locationVehicleLongitudeKey, float(l.vehicleLongitude)),
    ])
  );

let ofJson = str => Util.parseJsonList(str, decodeLocation);

let toJson = pList => Util.encodeJsonList(pList, encodeLocation);
