<script lang="ts">
    import Single from "./Single.svelte";
    import Sequencer from "./Sequencer.svelte";

    let isPaneSingle: boolean = true;

    function paneOne() {
        isPaneSingle = true;
    }
    function paneTwo() {
        isPaneSingle = false;
    }
    let visualisationComp;
</script>

<section>
    <div class="flex font-bold">
        <div on:click={paneOne} class="pane-title {isPaneSingle ? 'bg-gray-800': ''}">Single</div>
        <div on:click={paneTwo} class="pane-title {!isPaneSingle ? 'bg-gray-800': ''}">Sequencer</div>
    </div>
    <div class="border border-gray-600 w-full p-4">
        {#if isPaneSingle}
            <Single bind:this={visualisationComp}/>
        {:else }
            <Sequencer bind:this={visualisationComp}/>
        {/if}

    </div>
</section>

<style lang="postcss">
    .pane-title {
        @apply border border-gray-600 w-1/2 p-4 hover:cursor-pointer;
    }

</style>
