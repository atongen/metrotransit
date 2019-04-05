open Belt;

open Belt.Result;

let url = "https://svc.metrotransit.org/nextrip/providers?format=json";

type state =
  | NotAsked
  | Loading
  | Failure(string)
  | Success(list(Provider.t));

type action =
  | LoadProviders
  | LoadedProviders(list(Provider.t))
  | LoadProvidersFailed(string);

let component = ReasonReact.reducerComponent("ProviderSelect");

let menuItems = providers =>
  List.map(providers, (provider: Provider.t) =>
    <MaterialUi.MenuItem key=provider.id value=(`String(provider.id))>
      (ReasonReact.string(provider.name))
    </MaterialUi.MenuItem>
  );

let make = (~selected, ~setProvider, _childern) => {
  let providerChange = (evt, _el) => {
    let provider = ReactEvent.Form.target(evt)##value;
    setProvider(provider);
  };
  {
    ...component,
    initialState: () => NotAsked,
    reducer: (action, _state) =>
      switch (action) {
      | LoadProviders =>
        ReasonReact.UpdateWithSideEffects(
          Loading,
          (
            self =>
              Js.Promise.(
                Util.getCachedUrl(url)
                |> then_(jsonStr => Provider.ofJson(jsonStr) |> resolve)
                |> then_(result =>
                     switch (result) {
                     | Ok(providers) => resolve(self.send(LoadedProviders(providers)))
                     | Error(err) => resolve(self.send(LoadProvidersFailed(err)))
                     }
                   )
                |> ignore
              )
          ),
        )
      | LoadedProviders(providers) => ReasonReact.Update(Success(providers))
      | LoadProvidersFailed(err) => ReasonReact.Update(Failure(err))
      },
    didMount: self => self.send(LoadProviders),
    render: self =>
      switch (self.state) {
      | NotAsked => ReasonReact.null
      | Loading => <div> (ReasonReact.string("Loading providers...")) </div>
      | Failure(err) => <div> (ReasonReact.string("Something went wrong: " ++ err)) </div>
      | Success(providers) =>
        let selectedStr =
          switch (selected) {
          | Some(s) => s
          | None => ""
          };
        MaterialUi.(
          <form autoComplete="off">
            <FormControl>
              <InputLabel> (ReasonReact.string("Provider")) </InputLabel>
              <Select value=(`String(selectedStr)) onChange=providerChange> (menuItems(providers)) </Select>
            </FormControl>
          </form>
        );
      },
  };
};
