<script lang="ts">
    import Visualisation from "./Visualisation.svelte";
    import {onMount} from "svelte";
    import {serialHandler} from "./serial-handler";

    let running = 0;
    let isRecording = 0;
    let visualisationComp;
    let maximum = 1;
    let maximumTime = new Date();

    //let data = [20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20]
    let data = [368, 279, 353, 412, 20, 200, 20, 673, 456, 456, 500, 456, 20, 100, 20, 130]
    let baseline = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    let result = '0.0'

    const mapper = {'1':0,'2':1,'3':2,'4':3,
                    '7':4,'8':5,'9':6,'10':7,
                    '13':8,'14':9,'15':10,'16':11,
                    '19':12,'20':13,'21':14,'22':15}

    let savingBaseline: boolean = false;


    async function draw() {
        //visualisationComp.DrawBoxes([0,0,0,0])
        visualisationComp.DrawCurves([data.slice(0,4),data.slice(4,8),data.slice(8,12),data.slice(12,16)])
        if (serialHandler.isConnected & running) {
            serialHandler.read().then((results) => {
                performTask(results,
                    1,
                    function (items, index) {
                        if ((items[index] == 170) && running) {
                            if (((results[index + 1] & 0b11100000) == 160) & ((results[index + 2] & 0b11100000) == 224)) {
                                let from = (results[index + 1] & 0b11111)
                                let to = (results[index + 2] & 0b11111)
                                let msr = parseInt(Number(results[index + 4]).toString(2).padStart(8, "0") + Number(results[index + 3]).toString(2).padStart(8, "0"),2)

                                let mappedIndex = mapper[(results[index + 1] & 0b11111)]
                                //if (typeof mappedIndex == "undefined") return
                                //console.log(parseInt(header.slice(11,16),2))
                                //console.log(mappedIndex)

                                if ((msr > maximum) | (new Date().getSeconds() - maximumTime.getSeconds()) > 1)  {
                                    maximumTime = new Date()
                                    maximum = msr
                                }

                                msr -=  (baseline[mappedIndex])

                                let rs = Math.round(((msr-baseline[mappedIndex])/12536)*500)
                                if (rs > 500) rs = 500
                                if (rs < 20) rs = 20
                                data[mappedIndex] = rs
                                visualisationComp.DrawCurves([data.slice(0,4),data.slice(4,8),data.slice(8,12),data.slice(12,15)])
                                console.log([from,to,rs])

                                if (savingBaseline) {
                                    if (baseline[mappedIndex] < msr) baseline[mappedIndex] = msr
                                }



                                return true;
                            }
                        }
                        return false;
                    });
                draw();
            })
        }
    }

    async function start() {
        await serialHandler.write("\cq")
        //console.log(serialHandler.read())
        running = 1;
        draw()
    }

    function stop() {
        running = 0;
    }

    function saveBaseline() {
        savingBaseline = !savingBaseline
    }

    onMount(async () => {
        draw()
    });

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

<div class="grid grid-cols-2 gap-8">
    <Visualisation bind:this={visualisationComp} />
    <div>
        <h3>Settings</h3>
        <div class="grid grid-cols-2 gap-4 mb-4">
            <button on:click={start} type="button"
                    class="inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] {running ? 'bg-neutral-300 hover:bg-neutral-300 hover:cursor-not-allowed': ''}">
                Start
            </button>
            <button on:click={stop} type="button"
                    class="inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] {!running ? 'bg-neutral-300 hover:bg-neutral-300 hover:cursor-not-allowed': ''}">
                Stop
            </button>
        </div>

        <h3>Recorder</h3>
        <div class="mb-4">
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

        <h3>Baseline Settings</h3>
        <div class="mb-4">
            <div class="mb-2 flex flex-wrap justify-between gap-4">
                {#each baseline as value, index}
                    <div class="flex flex-row items-center">
                        <div class="flex items-center justify-center w-8 p-1 h-10 rounded-l-md {savingBaseline ? 'bg-red-600' : 'bg-green-600'}">{index}</div>
                        <input class="w-20 h-10 p-1 border-2 rounded-r-md bg-transparent {savingBaseline ? 'border-red-600' : 'border-green-600'}" type=number bind:value={value} min=0>
                    </div>
                {/each}
            </div>
            <button id="action-copy" type="button" on:click={saveBaseline}
                    class="col-span-1 inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)]">
                {savingBaseline ? 'Stop saving baseline' : 'Start saving baseline'}</button>

        </div>
    </div>

</div>



