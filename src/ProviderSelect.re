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

let make = (~selected: option(Provider.t), ~setProvider, _childern) => {
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
              ApiUri.loadProviders()
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
    | Loading => Util.typography("Loading providers...")
    | Failure(err) => Util.typography("Something went wrong: " ++ err)
    | Success(providers) =>
      let providerChange = (evt, _el) => {
        let providerId = ReactEvent.Form.target(evt)##value;
        let provider = List.getBy(providers, provider => provider.id == providerId);
        setProvider(provider);
      };
      let value =
        switch (selected) {
        | Some(provider) => `String(provider.id)
        | None => `String("all")
        };
      let style = ReactDOMRe.Style.make(~overflow="hidden", ());
      <form autoComplete="off">
        MaterialUi.(
          <FormControl fullWidth=true>
            <InputLabel> (ReasonReact.string("Provider")) </InputLabel>
            <Select value onChange=providerChange style> (menuItems(providers)) </Select>
          </FormControl>
        )
      </form>;
    },
};
