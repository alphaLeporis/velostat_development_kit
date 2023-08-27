# Svelte + TypeScript + Vite Project
Welcome to the VDK interface powered by Svelte, TypeScript and Vite! 
## Getting Started
### 1. Install Dependencies
Navigate to the project directory and install the required dependencies:

```sh
cd web
npm install
```

### 2. Run the Development Server
Launch the Vite development server with hot-reloading enabled:

```sh
npm run dev
```

This command will start the development server and open a new browser tab with your Svelte app.

## Project Structure

The project structure is organized as follows:

- `src/`: This directory contains your Svelte components, styles, and other assets.
- `public/`: Static files placed here will be served as-is. Useful for assets like images or fonts.
- `node_modules/`: The directory where npm packages are installed.
- `vite.config.ts`: Configuration file for Vite.

## TypeScript Support
This project is set up to work with TypeScript out of the box. TypeScript typings are automatically inferred for Svelte components, making your development experience more robust and error-free.

## Building for Production
To build your project for production, run the following command:

```sh
npm run build
```

This command will create an optimized production build in the `dist/` directory.

## Deployment
I have deployed this project to Netlify and can be accessed [here](https://vdk.louisdelooze.be).

## Customization

Feel free to customize this project to suit your needs. You can modify the Svelte components, add new routes, update styles, and more. Don't forget to refer to the [Svelte documentation](https://svelte.dev/docs) and [Vite documentation](https://vitejs.dev/) for more information.