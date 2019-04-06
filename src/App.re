type action =
  | Noop;

type state = string;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | Noop => ReasonReact.Update("hello")
    },
  initialState: () => "wow",
  render: _self => <div> <ConfigSelect /> </div>,
};
