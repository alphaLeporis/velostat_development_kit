<script lang="ts">
  import Connection from './lib/Connection.svelte'
  import Single from "./lib/Single.svelte";
  import Sequencer from "./lib/Sequencer.svelte";

  let isPaneSingle: boolean = true;

  function paneOne() {
    isPaneSingle = true;
  }
  function paneTwo() {
    isPaneSingle = false;
  }



</script>

<main>
  <div class="flex justify-between mb-12">
    <h1 class="">Velostat DEV kit</h1>
    <Connection/>
  </div>

    <div class="">
      <div class="flex font-bold">
        <div on:click={paneOne} class="pane-title {isPaneSingle ? 'bg-gray-800': ''}">Single</div>
        <div on:click={paneTwo} class="pane-title {!isPaneSingle ? 'bg-gray-800': ''}">Sequencer</div>
      </div>
      <div class="border border-gray-600 w-full p-4">
        {#if isPaneSingle}
          <Single/>
        {:else }
          <Sequencer/>
        {/if}
      </div>
    </div>
</main>

<style lang="postcss">
  .pane-title {
    @apply border border-gray-600 w-1/2 p-4 hover:cursor-pointer;
  }
</style>

