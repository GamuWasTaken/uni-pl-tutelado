import { Canvas } from '@react-three/fiber'
import { useRef } from 'react';

export default function MyThree() {
    const mountRef = useRef<HTMLDivElement>(null);
  return (<>
    <div id="canvas-container">      
    <div className="renderer-header">
        Output
    </div>
    <Canvas>
      <mesh>
        <boxGeometry args={[1, 1, 1]} />
        <meshPhongMaterial />
      </mesh>
      <mesh
        position={[2,0,0]}>
        <boxGeometry args={[1, 1, 1]} />
        <meshPhongMaterial />
      </mesh>
      <ambientLight intensity={0.1} />
      <directionalLight position={[0, 0, 5]} color="red" />
      <camera></camera>
    </Canvas>
    </div>
  </>
  )
}
