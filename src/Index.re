let mainStyle = ReactDOMRe.Style.make(~display="flex", ~marginLeft="auto", ~marginRight="auto", ());
let paperStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~flexDirection="column",
    ~alignItems="center",
    ~margin="24px",
    ~padding="9px",
    (),
  );

let content =
  <main style=mainStyle>
    MaterialUi.(
      <CssBaseline>
        <Paper style=paperStyle>
          <Typography variant=`H1> (ReasonReact.string("Metrotransit")) </Typography>
          <App />
        </Paper>
      </CssBaseline>
    )
  </main>;

ReactDOMRe.renderToElementWithId(content, "root");
