let width: Reprocessing_Common.glEnv => int;
let height: Reprocessing_Common.glEnv => int;
let mouse: Reprocessing_Common.glEnv => (int, int);
let pmouse: Reprocessing_Common.glEnv => (int, int);
let mousePressed: Reprocessing_Common.glEnv => bool;
let keyCode: Reprocessing_Common.glEnv => Reasongl.Gl.Events.keycodeT;
let size: width::int => height::int => Reprocessing_Common.glEnv => unit;
let resizeable: bool => Reprocessing_Common.glEnv => unit;
let frameRate: Reprocessing_Common.glEnv => int;
let frameCount: Reprocessing_Common.glEnv => int;
