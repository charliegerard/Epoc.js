# JavaScript framework for the Emotiv EPOC [WIP]

This framework provides an interface to access data from the Emotiv EPOC brain sensor using Node.js.

To use this framework, you're gonna need an EPOC / EPOC+ or Insight that you can buy [here](https://emotiv.com/).

Originally based on the [epocutils library](http://stephaneag.github.io/epocutils/) by [@StephaneAG](https://github.com/stephaneAG)

## Status:

* **Refactored to use latest SDK (v3.5.0).**

* Cognitive actions should now be working.

## How to Install

* Download the **Emotiv Community SDK v3.4.0** [here](https://github.com/Emotiv/community-sdk/releases) and copy the `edk.framework` file (community-sdk > lib > Mac > edk.framework) to `/Library/Frameworks` (on Mac).

* Either clone this repo and run

```
  npm install
```
or just run:

```
  npm install epocjs
```

## How to Use

When writing a program, use either the `connectToEmoComposer` or `connectToLiveData` function to use the emulator or the data coming from the device.

Example of simple program:

```JavaScript
  var Epoc = require('epocjs')();

  Epoc.connectToLiveData("<path to your profile file>", function(event){
    if(event.smile > 0){
      console.log('smiling')
    }
  })
```
or:

```JavaScript
  var Epoc = require('epocjs')();

  Epoc.connectToEmoComposer(function(event){
    if(event.smile > 0){
      console.log('smiling')
    }
  })
```

The path to your user file should be something like this */Users/< username >/Library/Application Support/Emotiv/Profiles/< filename >.emu*

## Events

---

#### Smile

If the value is superior to 0, the headset is detecting that the user is smiling.

```JavaScript
  if(event.smile > 0){
    // do something
  }
```

---

#### Looking Up / Down / Left / Right

The headset can detect the direction in which the user is looking:

```JavaScript
  if(event.lookingUp > 0){
    // do something
  }

  if(event.lookingDown > 0){
    // do something
  }

  if(event.lookingLeft > 0){
    // do something
  }

  if(event.lookingRight > 0){
    // do something
  }
```

---

#### Blink

Detect if the user blinked:

```JavaScript
  if(event.blink > 0){
    // do something
  }
```

---

#### Winking Left / Right

The headset can detect if the user is winking:

```JavaScript
  if(event.winkingLeft > 0){
    // do something
  }

  if(event.winkingRight > 0){
    // do something
  }
```

---

#### Laugh

Detect if the user is laughing:

```JavaScript
  if(event.laugh > 0){
    // do something
  }
```

---

#### Gyroscope

Detect changes in gyroscope data:

```JavaScript
  if(event.gyroX){
    // do something
  }

  if(event.gyroX){
    // do something
  }
```

---


#### Cognitive Actions

Actions available:

- push
- pull
- lift
- drop
- left
- right
- rotate left
- rotate right
- rotate clockwise
- rotate counter clockwise
- rotate reverse
- disappear

```JavaScript
  switch(cognitivAction){
    case 2:
      console.log('push')
      break;
    case 4:
      console.log('pull')
      break;
    case 8:
      console.log('lift')
      break;
    case 16:
      console.log('drop')
      break;
    case 32:
      console.log('left')
      break;
    case 64:
      console.log('right')
      break;
    case 128:
      console.log('rotate left')
      break;
    case 256:
      console.log('rotate right')
      break;
    case 512:
      console.log('rotate clockwise')
      break;
    case 1024:
      console.log('rotate counter clockwise')
      break;
    case 2048:
      console.log('rotate forwards')
      break;
    case 4096:
      console.log('rotate reverse')
      break;
    case 8192:
      console.log('disappear')
      break;
  }
```

---

## Special Thanks

Thanks to [Cam Swords](https://github.com/camswords) for his help :)

## License

Licensed under the MIT license.
