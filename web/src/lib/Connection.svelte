<script lang="ts">
    import { serialHandler } from './serial-handler';

    async function connectButtonElem() {
        await serialHandler.init();
        isConnected = serialHandler.isConnected;
    }

    let isConnected = serialHandler.isConnected;
</script>

<section>
    <button on:click={connectButtonElem} type="button"
            class="inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)]">
        Select port
    </button>
    {#if isConnected}
        <span class="ml-4 text-green-300 font-semibold">Connected</span>
    {:else }
        <span class="ml-4 text-red-400 font-semibold">Disconnected</span>
    {/if}


    <section class="hidden elements">
        <div class="element">
            <h3>Upload <a href="https://github.com/UnJavaScripter/web-serial-example/blob/master/web_serial_onboard_led.ino">the example code</a> to an Arduino</h3>
        </div>
        <div class="element">
            <h2>Connect to a serial port</h2>
            <button on:click={connectButtonElem} type="button"
                    class="inline-block rounded bg-neutral-50 px-6 pb-2 pt-2.5 text-xs font-medium uppercase leading-normal text-neutral-800 shadow-[0_4px_9px_-4px_#cbcbcb] transition duration-150 ease-in-out hover:bg-neutral-100 hover:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:bg-neutral-100 focus:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] focus:outline-none focus:ring-0 active:bg-neutral-200 active:shadow-[0_8px_9px_-4px_rgba(203,203,203,0.3),0_4px_18px_0_rgba(203,203,203,0.2)] dark:shadow-[0_4px_9px_-4px_rgba(251,251,251,0.3)] dark:hover:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:focus:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)] dark:active:shadow-[0_8px_9px_-4px_rgba(251,251,251,0.1),0_4px_18px_0_rgba(251,251,251,0.05)]">
                Connect
            </button>
        </div>
        <div class="element">
            <h2>Send a message</h2>
            <button class="message-button" data-value="0" disabled="true">Off</button>
            <button class="message-button" data-value="1" disabled="true">On</button>
        </div>
        <div class="element">
            <h2>Get a message back:</h2>
            <ol id="serial-messages-container"></ol>
        </div>
    </section>

</section>



