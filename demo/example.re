open ReactFrp.React;
open ReactReact;
open ReactReact.Utils;

module ShowName = {
  let vdomS = propsS =>
    S.map(
      ~eq=(_, _) => false,
      name =>
        if (name != "") {
          let greeting = {j|Hello, $name!|j};
          <p> {React.string(greeting)} </p>;
        } else {
          <p> {React.string("Hello, unknown person!")} </p>;
        },
      propsS,
    );
  [@react.component]
  let make = (~name) => componentFromSignal(vdomS, name);
};

module Timer = {
  let counter = ref(0);
  let (timeS, timeF) = S.create(counter^);
  let timeIncrement = () => {
    counter := counter^ + 1;
    timeF(counter^);
  };
  let timerId = Js.Global.setInterval(timeIncrement, 1000);
  let vdomS = _ =>
    S.map(
      ~eq=(_, _) => false,
      time => {
        let timeMessage = time == 1 ? "second" : "seconds";
        let message = {j|You've spent $time $timeMessage on this page!|j};
        <div> {React.string(message)} </div>;
      },
      timeS,
    );
  [@react.component]
  let make = () => componentFromSignal(vdomS, ());
};

module Input = {
  let (nameS, nameF) = S.create("");
  let vdomS = _ =>
    S.map(
      ~eq=(_, _) => false,
      name =>
        <div>
          <input type_="text" onChange={ev => emitEventToStream(nameF, ev)} />
          <ShowName name />
          <Timer />
        </div>,
      nameS,
    );
  [@react.component]
  let make = () => componentFromSignal(vdomS, ());
};

// ReactDOM.renderToElementWithId(<Input />, "index");
switch(ReactDOM.querySelector("#index")) {
| Some(root) => ReactDOM.render(<Input />, root)
| None => () // do nothing
}