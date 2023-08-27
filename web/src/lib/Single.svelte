<script lang="ts">
    import { LinkedChart, LinkedLabel, LinkedValue } from "svelte-tiny-linked-charts"
    import Visualisation from "./Visualisation.svelte";
    import {serialHandler} from "./serial-handler";
    import {onMount} from "svelte";
    import { Confirm } from 'svelte-confirm'


    let vPlus = 23;
    let vMinus = 7;
    let sPlus = 15;
    let sMinus = 14;
    let oldTW;

    let result = '0.0'
    let average = '0.0'

    let running = 0;
    let isRecording = 0;

    let data = {}

    let visualisationComp;
    async function draw() {
        visualisationComp.DrawBoxes([vPlus, vMinus, sPlus, sMinus])
        //serialHandler.write("i")
        let tw = "\c"+vPlus.toString().padStart(2, "0") + vMinus.toString().padStart(2, "0") + sPlus.toString().padStart(2, "0") + sMinus.toString().padStart(2, "0")
        if (oldTW !== tw) {
            console.log(tw)
            await serialHandler.write(tw);
            oldTW = tw
        }
        if (serialHandler.isConnected & running) {
            serialHandler.read().then((results) => {
                performTask(results,
                    1,
                    function (items, index) {
                        if ((items[index] == 170) && running) {
                            if (((results[index + 1] & 0b11100000) == 160) & ((results[index + 2] & 0b11100000) == 224)) {
                                //let from = (results[index + 1] & 0b11111)
                                //let to = (results[index + 2] & 0b11111)
                                let msr = Number(results[index + 4]).toString(2).padStart(8, "0") + Number(results[index + 3]).toString(2).padStart(8, "0")
                                average = ( ( Number.parseFloat(average) + (3.1 * parseInt(msr, 2) / 65536) ) / 2).toFixed(2)
                                result = Number.parseFloat(String(3.1 * parseInt(msr, 2) / 65536)).toFixed(2)
                                let now = new Date().toLocaleTimeString()
                                if (data[now]) {
                                   data[now] = Number.parseFloat(((data[now]+Number.parseFloat(result))/2).toFixed(2))
                                } else {
                                    data[now] = Number.parseFloat(result)
                                }
                                if (Object.keys(data).length > 128) {
                                    delete data[Object.keys(data).at(0)]
                                }
                                console.log(data)
                                return true;
                            }
                        }
                        return false;
                    });
                draw();

            })
        }

    }
    onMount(async () => {
        draw()
    });
    function stop() {
        running = 0
    }

    async function reset() {
        average = '0.0'
        data = {}
    }
    async function start() {
        await serialHandler.write("\ci")
        running = 1;
        draw()
    }

    async function recordStart() {
        await serialHandler.write("\cr")
        isRecording = 1
    }

    function recordStop() {
        serialHandler.write("\cs")
        isRecording = 0
    }

    function performTask(items, numToProcess, processItem) {
        var pos = 0;
        // This is run once for every numToProcess items.
        function iteration() {
            if (processItem(items, pos)) {
                pos += 5
            } else {
                pos += 1
            }
            if (pos < items.length)
                setTimeout(iteration, 10); // Wait 10 ms to let the UI update.
        }
        iteration();
    }

</script>
<Confirm
        let:confirm="{confirmThis}"
>
<div class="grid grid-cols-2 gap-8">
    <Visualisation bind:this={visualisationComp} />
    <div>
        <h3>Select port</h3>
        <div class="grid grid-cols-2 gap-2">
            <div class="hover:cursor-pointer flex gap-2">
                <input class="w-10 h-10 bg-red-600 p-1" type=number bind:value={vPlus} min=0 max=23>
                <p>V+</p>
            </div>
            <div class="hover:cursor-pointer flex gap-2">
                <input class="w-10 h-10 bg-red-600 p-1" type=number bind:value={vMinus} min=0 max=23>
                <p>V-</p>
            </div>
            <div class="hover:cursor-pointer flex gap-2">
                <input class="w-10 h-10 bg-green-600 p-1" type=number bind:value={sPlus} min=0 max=23>
                <p>SEN+</p>
            </div>
            <div class="hover:cursor-pointer flex gap-2">
                <input class="w-10 h-10 bg-green-600 p-1" type=number bind:value={sMinus} min=0 max=23>
                <p>SEN-</p>
            </div>
            <button id="action-start" type="button" on:click={start}
                    class="col-span-1 inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)]">
                Start</button>
            <button id="action-stop" type="button" on:click={stop}
                    class="col-span-1 inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)]">
                Stop</button>
            <button id="action-reset" type="button" on:click="{() => confirmThis(reset)}"
                    class="col-span-2 inline-block rounded bg-transparent border border-4 border-red-600 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal ">
                Reset</button>
        </div>

        <h3>Measurements</h3>
        <div class="grid grid-cols-2 gap-4 pb-4">
            <div class="h-16 bg-transparant border-4 border-white p-2 pt-0">
                <span class="text-xs">live</span>
                <p class="my-auto font-bold">{result}</p>
            </div>
            <div class="h-16 bg-transparant border-4 border-white p-2 pt-0">
                <span class="text-xs">average (since start)</span>
                <p class="my-auto font-bold">{average}</p>
            </div>
        </div>

        <div class="pb-1 float-right">
            <LinkedLabel linked="table" empty="Last value"/>
        </div>
        <div class="items-center border-b-2 border-white w-full flex mb-4 gap-2">
            <div class="w-11/12">
                <LinkedChart barMinWidth="1" data={ data } linked="table" uid="table-6" />
            </div>
            <div  class="w-1/12 font-bold">
                <LinkedValue uid="table-6" empty={ Object.values(data).at(-2) || 0 }  />
            </div>
        </div>

        <h3>Recorder</h3>
        <div>
            <button id="action-record-start" type="button" on:click={recordStart}
                    class="col-span-1 inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)]">
                Start</button>
            <button id="action-record-stop" type="button" on:click={recordStop}
                    class="col-span-1 inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)]">
            Stop</button>
            {#if isRecording}
                <span class="pl-4 text-green-300 font-semibold">Recording</span>
            {:else }
                <span class="pl-4 text-red-400 font-semibold">Not recording</span>
            {/if}

        </div>


    </div>
</div>
</Confirm>


