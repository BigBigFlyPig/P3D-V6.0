package
{
	import Prepar3D;
	
	public final class P3DSound 
	{
		private var id: int;
		private var loop: Boolean;
		private var play: Boolean;

		public function P3DSound(filename:String) 
		{
			id = Prepar3D.SoundCreate(filename);
		}
		
		/** 
		* Play non looping sound
		*/
		public function Play()
		{
			loop = false;
			play = true;
			Update();
		}
		
		/** 
		* Play looping sound
		*/
		public function PlayLooped()
		{
			loop = true;
			play = true;
			Update();
		}
		
	    /** 
		* Stop sound
		*/
		public function Stop()
		{
			play = false;
			Update();
		}
		
	    /** 
		* Detroy sound.  This will clear out the sound instance in the Prepar3d application.
		* After calling this, this class will no longer be usable.  This is provided because action Script classes are
		* garbage collected by the scaleform/flash virtual machine and do not have destructors.
		*/
		public function Destroy()
		{
			Prepar3D.SoundDestroy(id);
		}
				
		/** 
		* Update sound state in prepar3D.  Used internally by this class to update play/loop state.
		*/
		private function Update()
		{
			Prepar3D.SoundSetProperties(id,play,loop);
		}
	}
}