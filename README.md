# bs-download

BuckleScript bindings of [download](https://www.npmjs.com/package/download) module.

## Install

```
yarn add @dck/bs-download
```

Add it to `bs-dependencies` in your `bsconfig.json` :

```json
{
  "bs-dependencies": ["@dck/bs-download"]
}
```

## Usage

### Basic

```reason
open BsDownload;

download(
  "http://download-link.something",
  "destination/folder",
  DownloadOptions.t(~extract=false, ~filename="custom filename")
)
|> Js.Promise.then_(data => {
  Js.log(data->DownloadResponse.filepathGet);
  Js.Promise.resolve();
})
|> ignore;
```

### With stream

```reason
open BsDownload;

let downloadedContent = ref(0);

download(
  "http://download-link.something",
  "destination/folder",
  DownloadOptions.t(~extract=false, ~filename="custom filename")
)
|> Js.Promise.then_(data => {
  Js.log(data->DownloadResponse.filepathGet);
  Js.Promise.resolve();
})
|> on(`response(res => {
  let contentLength =
      res
      ->DownloadResponse.headers
      ->DownloadResponse.getContentLength("content-length")
      ->Js.Nullable.toOption
      ->Belt.Option.getWithDefault(0);
  Js.log(contentLength);
}))
|> on(`data(chunk => {
  downloadedContent := downloadedContent^ + chunk->DownloadResponse.Chunk.length;
  Js.log(downloadedContent);
}))
|> on(`end_(() => {
  Js.log("End.");
}))
|> ignore;
```
