const $ = s => document.querySelector(s);
let ctx, master, noise, playing = false, timerId, elapsed = 0, noteTimer, sessionTimer;
let state = { mood:'calm', density:1, warmth:62, preset:'tidal' };
const presets = {
 tidal:{name:'TIDAL FOCUS', desc:'E minor · soft granular pad', notes:[164.81,196,246.94,293.66,329.63], wave:'sine'},
 library:{name:'QUIET LIBRARY', desc:'C major · felt keys', notes:[130.81,164.81,196,261.63,329.63], wave:'triangle'},
 rain:{name:'RAIN WINDOW', desc:'D minor · warm haze', notes:[146.83,174.61,220,293.66,349.23], wave:'sine'},
 night:{name:'NIGHT DESK', desc:'A minor · low hum', notes:[110,130.81,164.81,220,261.63], wave:'triangle'}
};
function initAudio(){
 if(ctx) return; ctx = new AudioContext();
 master=ctx.createGain(); master.gain.value=.22; master.connect(ctx.destination);
 // Continuous airy texture, gently filtered
 const buffer=ctx.createBuffer(1,ctx.sampleRate*2,ctx.sampleRate); const data=buffer.getChannelData(0); for(let i=0;i<data.length;i++)data[i]=(Math.random()*2-1)*.25;
 noise=ctx.createBufferSource(); noise.buffer=buffer; noise.loop=true; const filter=ctx.createBiquadFilter(); filter.type='lowpass'; filter.frequency.value=950; const ng=ctx.createGain();ng.gain.value=.045; noise.connect(filter).connect(ng).connect(master); noise.start();
}
function tone(freq, when, length=4, volume=.07){
 const o=ctx.createOscillator(), g=ctx.createGain(), f=ctx.createBiquadFilter();
 const warm=Number(state.warmth); f.type='lowpass'; f.frequency.setValueAtTime(500 + warm*24,when); f.Q.value=1.2; o.type=presets[state.preset].wave;
 o.frequency.setValueAtTime(freq,when); o.detune.value=(Math.random()-.5)*8;
 g.gain.setValueAtTime(.0001,when);g.gain.linearRampToValueAtTime(volume,when+.8);g.gain.exponentialRampToValueAtTime(.0001,when+length);
 o.connect(f).connect(g).connect(master);o.start(when);o.stop(when+length+.1);
}
function schedule(){
 if(!playing)return; const p=presets[state.preset], now=ctx.currentTime; let count=state.density===1?1:state.density===2?2:3;
 for(let i=0;i<count;i++){const n=p.notes[Math.floor(Math.random()*p.notes.length)]; tone(n/(i===0?1:2),now+i*.35, state.mood==='focus'?2.7:4.5,.05/(i+1));}
 const delay = state.mood==='focus'?2400:state.mood==='drift'?4900:3700; noteTimer=setTimeout(schedule,delay);
}
function format(seconds){return `${String(Math.floor(seconds/60)).padStart(2,'0')}:${String(seconds%60).padStart(2,'0')}`}
function start(){initAudio();ctx.resume();playing=true;document.body.classList.add('playing');$('#playButton').setAttribute('aria-label','Pause sound');$('#statusText').textContent='PLAYING';$('#footerState').textContent='SOUND ON'; schedule(); timerId=setInterval(()=>{$('#timer').textContent=format(++elapsed)},1000); const mins=+document.querySelector('.session.active').dataset.minutes;if(mins)sessionTimer=setTimeout(stop,mins*60000)}
function stop(){playing=false;clearTimeout(noteTimer);clearTimeout(sessionTimer);clearInterval(timerId);document.body.classList.remove('playing');$('#playButton').setAttribute('aria-label','Start sound');$('#statusText').textContent='PAUSED';$('#footerState').textContent='SOUND OFF'}
$('#playButton').onclick=()=>playing?stop():start();
document.querySelectorAll('.mood').forEach(b=>b.onclick=()=>{document.querySelector('.mood.active').classList.remove('active');b.classList.add('active');state.mood=b.dataset.mood;$('#moodValue').textContent=state.mood.toUpperCase();});
$('#density').oninput=e=>{state.density=+e.target.value;$('#densityValue').textContent=['','SPARSE','FLOW','RICH'][state.density]};
$('#warmth').oninput=e=>{state.warmth=e.target.value;$('#warmthValue').textContent=`${state.warmth}%`};
document.querySelectorAll('.session').forEach(b=>b.onclick=()=>{document.querySelector('.session.active').classList.remove('active');b.classList.add('active');clearTimeout(sessionTimer);if(playing&&+b.dataset.minutes)sessionTimer=setTimeout(stop,+b.dataset.minutes*60000)});
document.querySelectorAll('.preset-card').forEach(b=>b.onclick=()=>{document.querySelector('.preset-card.selected').classList.remove('selected');b.classList.add('selected');state.preset=b.dataset.preset;const p=presets[state.preset];$('#presetName').textContent=p.name;$('#soundDescription').textContent=p.desc;if(playing){clearTimeout(noteTimer);schedule()}});
$('#newPattern').onclick=()=>{if(playing){clearTimeout(noteTimer);schedule()} else {$('#footerState').textContent='PATTERN READY';setTimeout(()=>$('#footerState').textContent='SOUND OFF',1200)}};
$('#themeButton').onclick=()=>document.body.classList.toggle('dark');
