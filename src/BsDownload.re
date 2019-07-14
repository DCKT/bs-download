open BsNode;

module DownloadOptions = {
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    extract: bool,
    [@bs.optional]
    filename: string,
    [@bs.optional]
    proxy: string,
  };
};

module DownloadStatus = {
  [@bs.deriving jsConverter]
  type t = [
    | [@bs.as "waiting"] `waiting
    | [@bs.as "pending"] `pending
    | [@bs.as "completed"] `completed
    | [@bs.as "error"] `error
  ];
};

module DownloadResponse = {
  type t;
  type headers;
  [@bs.deriving abstract]
  type payload = {
    data: string,
    filename: string,
    filepath: string,
  };

  module Chunk = {
    type t;

    [@bs.get] external length: t => int = "length";
  };
  type chunk;

  [@bs.get] external headers: t => headers = "headers";

  [@bs.get_index]
  external getContentLength: (headers, string) => Js.Nullable.t(int) = "";

  [@bs.module "download"] external getFilename: t => string = "getFilename";
};

type downloadResponse = Js.Promise.t(DownloadResponse.payload);

[@bs.module "download"]
external download: (string, string, DownloadOptions.t) => downloadResponse =
  "download";

[@bs.module "download"]
external downloadAsStream: string => Fs.Stream.t = "download";

[@bs.send.pipe: downloadResponse]
external on:
  (
  [@bs.string]
  [
    | `response(DownloadResponse.t => unit)
    | `data(DownloadResponse.Chunk.t => unit)
    | `error(string => unit)
    | [@bs.as "end"] `end_(unit => unit)
  ]
  ) =>
  downloadResponse =
  "on";