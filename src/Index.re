let mainStyle = ReactDOMRe.Style.make(~width="100%", ~display="block", ~marginLeft="auto", ~marginRight="auto", ());

let content =
  <main style=mainStyle>
    MaterialUi.(
      <CssBaseline> <Typography variant=`H1> (ReasonReact.string("Metrotransit")) </Typography> <App /> </CssBaseline>
    )
  </main>;

ReactDOMRe.renderToElementWithId(content, "root");
