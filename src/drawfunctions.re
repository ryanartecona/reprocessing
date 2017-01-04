open Common;

open Glloader;

open Glhelpers;

open Utils;

module P = {
  let width env => (!env).size.width;
  let height env => (!env).size.height;
  let mouse env => (!env).mouse.pos;
  let pmouse env => (!env).mouse.prevPos;
  let mousePressed env => (!env).mouse.pressed;
  let background (env: ref glEnv) (c: colorT) => env := {...!env, currBackground: c};
  let fill (env: ref glEnv) (c: colorT) => env := {...!env, currFill: c};
  let frameRate (env: ref glEnv) => (!env).frame.rate;
  let frameCount (env: ref glEnv) => (!env).frame.count;
  let size (env: ref glEnv) width height => {
    Gl.Window.setWindowSize window::(!env).window ::width ::height;
    resetSize env width height
  };
  let rect (env: ref glEnv) x y width height => {
    let vertexBuffer =
      makeRectVertexBuffer
        (float_of_int @@ x + width, float_of_int @@ y + height)
        (float_of_int x, float_of_int @@ y + height)
        (float_of_int @@ x + width, float_of_int y)
        (float_of_int x, float_of_int y)
        (!env).currFill;
    drawVertexBuffer ::vertexBuffer mode::Constants.triangle_strip count::4 !env
  };
  let resizeable (env: ref glEnv) resizeable =>
    env := {...!env, size: {...(!env).size, resizeable}};
  let rectf (env: ref glEnv) x y width height => {
    let vertexBuffer =
      makeRectVertexBuffer
        (x +. width, y +. height) (x, y +. height) (x +. width, y) (x, y) (!env).currFill;
    drawVertexBuffer ::vertexBuffer mode::Constants.triangle_strip count::4 !env
  };
  let loadImage (env: ref glEnv) filename :imageT => {
    let imageRef = ref None;
    Gl.loadImage
      ::filename
      callback::(
        fun imageData =>
          switch imageData {
          | None => assert false
          | Some img =>
            let env = !env;
            let textureBuffer = Gl.createTexture context::env.gl;
            imageRef := Some {img, textureBuffer};
            Gl.bindTexture context::env.gl target::Constants.texture_2d texture::textureBuffer;
            Gl.texImage2DWithImage context::env.gl target::Constants.texture_2d level::0 image::img;
            Gl.texParameteri
              context::env.gl
              target::Constants.texture_2d
              pname::Constants.texture_mag_filter
              param::Constants.linear;
            Gl.texParameteri
              context::env.gl
              target::Constants.texture_2d
              pname::Constants.texture_min_filter
              param::Constants.linear;
          }
      )
      ();
    imageRef
  };
  let image (env: ref glEnv) img x y =>
    switch !img {
    | None => print_endline "image not ready yet, just doing nothing :D"
    | Some {img, textureBuffer} =>
      let env = !env;
      let width = Gl.getImageWidth img;
      let height = Gl.getImageHeight img;
      let (x1, y1) = (float_of_int @@ x + width, float_of_int @@ y + height);
      let (x2, y2) = (float_of_int x, float_of_int @@ y + height);
      let (x3, y3) = (float_of_int @@ x + width, float_of_int y);
      let (x4, y4) = (float_of_int x, float_of_int y);
      let verticesColorAndTexture = [|
        x1,
        y1,
        0.0,
        0.0,
        0.0,
        0.0,
        1.,
        1.0,
        1.0,
        x2,
        y2,
        0.0,
        0.0,
        0.0,
        0.0,
        1.,
        0.0,
        1.0,
        x3,
        y3,
        0.0,
        0.0,
        0.0,
        0.0,
        1.,
        1.0,
        0.0,
        x4,
        y4,
        0.0,
        0.0,
        0.0,
        0.0,
        1.,
        0.0,
        0.0
      |];
      drawVertexBuffer
        vertexBuffer::verticesColorAndTexture
        mode::Constants.triangle_strip
        count::4
        textureFlag::1.0
        ::textureBuffer
        env
    };
  let background env color => {
    let w = width env;
    let h = height env;
    let oldEnv = !env;
    fill env color;
    rect env 0 0 w h;
    env := oldEnv
  };
  let clear env => Gl.clear (!env).gl (Constants.color_buffer_bit lor Constants.depth_buffer_bit);
  let stroke env color => env := {...!env, stroke: {...(!env).stroke, color}};
  let strokeWeight env weight => env := {...!env, stroke: {...(!env).stroke, weight}};
  let line env (xx1, yy1) (xx2, yy2) => {
    let dx = xx2 - xx1;
    let dy = yy2 - yy1;
    let mag = PUtils.dist (xx1, yy1) (xx2, yy2);
    let radius = float_of_int (!env).stroke.weight /. 2.;
    let xthing = PUtils.round (float_of_int dy /. mag *. radius);
    let ything = PUtils.round (-. float_of_int dx /. mag *. radius);
    let x1 = float_of_int xx1 +. xthing;
    let y1 = float_of_int yy1 +. ything;
    let x2 = float_of_int xx2 +. xthing;
    let y2 = float_of_int yy2 +. ything;
    let x3 = float_of_int xx2 -. xthing;
    let y3 = float_of_int yy2 -. ything;
    let x4 = float_of_int xx1 -. xthing;
    let y4 = float_of_int yy1 -. ything;
    let vertexBuffer =
      makeRectVertexBuffer (x2, y2) (x3, y3) (x1, y1) (x4, y4) (!env).stroke.color;
    drawVertexBuffer ::vertexBuffer mode::Constants.triangle_strip !env
  };
  let ellipse env a b c d => drawEllipseInternal !env a b c d;
};