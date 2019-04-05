let setExpiringItem: (string, string, float) => string;

let getExpiringItem: string => option(string);

let getSetExpiringItem: (string, unit => Js.Promise.t(string), float) => Js.Promise.t(string);
