# Arenagame — design brief

## Core loop

Wake in the sealed station → search rooms and listen for clues → scavenge the fuel canister and keycard → restore generator power → evade the stalker on the route to the sea-facing exit → reach the boat escape point. The initial code provides system hooks, not a finished playable level or UI.

## Horror pacing rules

- Alternate safe-but-uncertain exploration with short, readable threat escalations; avoid continuous pursuit.
- Use darkness, distant movement, radio fragments and changed environmental states before direct attacks.
- Give players a brief recovery window after a chase. Do not make every sound a threat cue.
- Fear should communicate danger, not obscure controls: use gradual movement and presentation changes, with a floor on movement speed.
- Keep key objective locations discoverable through environmental landmarks, not waypoint spam.

## Entity behaviour

`AStalkerEntity` patrols a designer-authored list of point actors, investigates explicit noise reports, and switches to a chase while it has line of sight to the player. After losing sight it searches the last seen location briefly, then returns to patrol. It uses `AAIController` navigation requests; place a Nav Mesh Bounds Volume and build navigation in the Editor. No navmesh or enemy animation asset is included. Add an AI Controller/BP and tune sight, speed and patrol points in a child Blueprint.

## Research station level brief

Build a compact loop around a flooded lower service wing, a central lab corridor and an elevated radio/control room. The wake-up room should be a safe tutorial pocket; the generator room should require the fuel; the keycard should gate a noisy shortcut; the boat exit should be visible or audible before it is reachable. Include at least two loops/alternate routes so a chase has choices. Use storm-facing windows sparingly as silhouettes and landmarks.

## Asset checklist for Unreal Editor

- [ ] Create a playable station map, floors/walls/doors, collision and a boat escape area.
- [ ] Create a first-person arms/weaponless camera presentation and flashlight mesh; optionally assign a camera shake Blueprint.
- [ ] Create a child `BP_StalkerEntity`, add an AI Controller and patrol point actors; add animation/montage assets later.
- [ ] Add a Nav Mesh Bounds Volume, build navigation, and verify paths around doors and stairs.
- [ ] Place Directional Light and Exponential Height Fog, then assign them to `AStormLightingController`.
- [ ] Create a child flicker light or attach `UFlickeringLightComponent` to selected station lights.
- [ ] Create `AAmbientAudioController` Blueprint and assign wind, heartbeat, entity-stinger and radio-static Sound Cues.
- [ ] Create fuel/keycard pickup Blueprints and bind them to an `AObjectiveManager`; add interactables for generator and escape.
- [ ] Create Input Action assets only if replacing the supplied runtime input setup; otherwise no input assets are required.
- [ ] Add props, decals, materials, storm audio, UI, accessibility options and save/checkpoint flow after the prototype loop is verified.
