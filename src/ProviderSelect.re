open Belt;

open Belt.Result;

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

let allProvider: Provider.t = {id: "all", name: "All"};

let menuItems = providers =>
  List.map(List.add(providers, allProvider), (provider: Provider.t) =>
    <MaterialUi.MenuItem key=provider.id value=(`String(provider.id))>
      (ReasonReact.string(provider.name))
    </MaterialUi.MenuItem>
  );

let make = (~selected, ~setProvider, _childern) => {
  let providerChange = (evt, _el) => {
    let provider = ReactEvent.Form.target(evt)##value;
    let maybeProvider =
      if (provider == "" || provider == "all") {
        None;
      } else {
        Some(provider);
      };
    setProvider(maybeProvider);
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
                Util.getCachedUrl(ApiUri.(toString(ProvidersUri)))
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
        let value =
          switch (selected) {
          | Some(s) => `String(s)
          | None => `String("all")
          };
        MaterialUi.(
          <form autoComplete="off">
            <FormControl>
              <InputLabel> (ReasonReact.string("Provider")) </InputLabel>
              <Select value onChange=providerChange>
                (menuItems(providers))
              </Select>
            </FormControl>
          </form>
        );
      },
  };
};
