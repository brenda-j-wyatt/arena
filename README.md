# Study Current

A calm, browser-based generative synth for study sessions and deep work. **Study Current** creates slow-moving ambient music that evolves while you work, directly in the browser.

## Features

- **Generative ambient audio** powered by the Web Audio API — no audio files or backend required.
- Four distinct listening spaces:
  - **Tidal Focus** — slow pulse in E minor
  - **Quiet Library** — felt-key inspired tones in C major
  - **Rain Window** — warm hazy tones in D minor
  - **Night Desk** — a low, quiet hum in A minor
- **Mood controls** for Calm, Drift, and Focus behavior.
- **Density** control for sparse, flowing, or richer arrangements.
- **Warmth** control to move from glassy to amber-toned filtering.
- Optional **25- and 50-minute sessions**, plus an elapsed-time display.
- A **New Pattern** control to immediately introduce a fresh arrangement.
- Responsive interface with a dark color mode.

## Run locally

### Prerequisites

- [Node.js](https://nodejs.org/) 18 or newer
- npm

### Install and start

```bash
npm install
npm run start
```

Then open the address Vite prints in the terminal (normally `http://localhost:5173`). Click the play button to allow the browser to start audio playback.

## How it works

Study Current uses the native browser [Web Audio API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Audio_API). Each musical event is generated from the selected preset's note collection, with randomized timing and slight detuning. Mood, density, and warmth modify the event scheduling and the synth/filter behavior in real time.

## Project structure

```text
├── index.html       # Application markup
├── style.css        # Responsive visual design
├── app.js           # Synth engine and interactive controls
├── vite.config.js   # Local development server settings
└── package.json     # Scripts and development dependencies
```

## Notes

Audio playback needs a user gesture, so the synth starts only after pressing the play control. Headphones are recommended for the full effect.

---

## Birthday card for Jayne

A separate, self-contained deliverable lives in [`birthday-card/`](birthday-card/):
a single-file interactive birthday card that can be emailed as an attachment or sent as
a link, and opens on an iPhone, an iPad, or a desktop browser.

- The card: `birthday-card/jayne-birthday-card.html` (open it directly — no build step)
- Instructions, including how to personalise and send it: `birthday-card/README.md`
- While `npm run start` is running, it is served at
  `/birthday-card/jayne-birthday-card.html`, or run `npm run card` to open it locally.
