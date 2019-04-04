let setExpiringItem: (string, string, float) => unit;

let getExpiringItem: string => option(string);

let getSetExpiringItem: (string, unit => option(string), float) => option(string);
