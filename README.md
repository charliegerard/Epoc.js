# JavaScript framework for the Emotiv EPOC [WIP]

This framework provides an interface to access data from the Emotiv EPOC brain sensor using Node.js.

To use this framework, you're gonna need an EPOC or EPOC+ that you can buy [here](https://emotiv.com/).

## Status:

* ** Refactored to use latest SDK – Working with EmoComposer but still need to test while wearing the sensor. **

* Known issues:
  * Cognitive Actions are not working at the moment when wearing the sensor.
  * Need to provide a way for users to provide the path to their own user data file when launching the program.

## How to Install

If you haven't already downloaded the SDK, clone or download [this repo](https://github.com/Emotiv/community-sdk) and copy the `edk.framework` to `/Library/Frameworks` (on Mac).

In your terminal, run:

```
  npm install epocjs
```


## How to Use

Example of simple program:

```
  var Epoc = require('epocjs')();

  Epoc.connect(function(event){
    if(event.smile > 0){
      console.log('smiling')
    }
  })
```

## Events

---

#### Smile

If the value is superior to 0, the headset is detecting that the user is smiling.

```
  if(event.smile > 0){
    // do something
  }
```

---

#### Looking Up / Down / Left / Right

The headset can detect the direction in which the user is looking:

```
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

```
  if(event.blink > 0){
    // do something
  }
```

---

#### Winking Left / Right

The headset can detect if the user is winking:

```
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

```
  if(event.laugh > 0){
    // do something
  }
```

---


#### Cognitive Actions (Only working with the EmoComposer at the moment)

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

```
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
    case 8192:
      console.log('disappear')
  }

```

---

## Next steps

* Add Gyroscope tracking.
* Get cognitive actions working.
* Add option for Insight device.

## Special Thanks

Thanks to [Cam Swords](https://github.com/camswords) for his help :)

## License

Licensed under the MIT license.
