type t =
  | UniversityOfMinnesota
  | AirportMAC
  | Other
  | PriorLake
  | ScottCounty
  | MetroTransitMetCouncil
  | SouthWestTransit
  | MetroTransit
  | MinnesotaValley
  | Plymouth
  | MetCouncil
  | MapleGrove;

let to_int =
  fun
  | UniversityOfMinnesota => 1
  | AirportMAC => 2
  | Other => 3
  | PriorLake => 4
  | ScottCounty => 5
  | MetroTransitMetCouncil => 6
  | SouthWestTransit => 7
  | MetroTransit => 8
  | MinnesotaValley => 9
  | Plymouth => 10
  | MetCouncil => 11
  | MapleGrove => 12;

let of_int =
  fun
  | 1 => Some(UniversityOfMinnesota)
  | 2 => Some(AirportMAC)
  | 3 => Some(Other)
  | 4 => Some(PriorLake)
  | 5 => Some(ScottCounty)
  | 6 => Some(MetroTransitMetCouncil)
  | 7 => Some(SouthWestTransit)
  | 8 => Some(MetroTransit)
  | 9 => Some(MinnesotaValley)
  | 10 => Some(Plymouth)
  | 11 => Some(MetCouncil)
  | 12 => Some(MapleGrove)
  | _ => None;

let to_string =
  fun
  | UniversityOfMinnesota => "University of Minnesota"
  | AirportMAC => "Airport (MAC)"
  | Other => "Other"
  | PriorLake => "Prior Lake"
  | ScottCounty => "Scott County"
  | MetroTransitMetCouncil => "Metro Transit/Met Council"
  | SouthWestTransit => "SouthWest Transit"
  | MetroTransit => "Metro Transit"
  | MinnesotaValley => "Minnesota Valley"
  | Plymouth => "Plymouth"
  | MetCouncil => "Met Council"
  | MapleGrove => "Maple Grove";
