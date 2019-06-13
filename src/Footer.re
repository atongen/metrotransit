let component = ReasonReact.statelessComponent("Footer");

let footerStyle = ReactDOMRe.Style.make(~marginTop="12px", ~textAlign="center", ());

let make = _children => {
  ...component,
  render: _self =>
    MaterialUi.(
      <footer style=footerStyle>
        <Typography>
          (ReasonReact.string("Built by "))
          <a target="_blank" href="https://www.andrewtongen.net/"> (ReasonReact.string("Andrew Tongen")) </a>
          (ReasonReact.string(" | "))
          <a target="_blank" href="https://github.com/atongen/metrotransit"> (ReasonReact.string("Source Code")) </a>
        </Typography>
      </footer>
    ),
};
