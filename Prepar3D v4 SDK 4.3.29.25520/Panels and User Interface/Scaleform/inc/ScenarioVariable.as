package
{
	import Prepar3D;

	/** ScenarioVariable
	* This is a helper class that encapsulates getting and setting scenario variables by use of accessors.
	*/
	public class ScenarioVariable
	{ 
		private var m_ResolvedName:String = ""; 
		private var m_Name:String = ""; 
		private var m_UnitType:String = ""; 
		
		private var m_InstanceName:String = ""; 
		private var m_UseAssociatedInstanceName:Boolean = true; 
		
		/** 
		* ScenarioVariable This constructor intializes the class with all of the information necessary to start querying
		* for the value of the scenario variable.
		*
		* @param name The name of the scenario variable
		* @param unitType The unit type of the scenario variable. See the <b>Units Of Measurement</b> document for 
		* a list of units.
		*/
		public function ScenarioVariable(name : String, unitType : String)
		{
			m_UnitType = unitType;
			UseAssociatedInstanceName = true;
			Name = name;
		}
		
		/** 
		* UseAssociatedInstanceName This function determines whether to use the instance name of the current movie
		* when accessing the variable
		*
		* @param useAssociatedInstanceName A boolean determining whether the instance name should be appended in the 
		* lookup.
		*/
		public function set UseAssociatedInstanceName(useAssociatedInstanceName:Boolean):void
		{ 
			m_UseAssociatedInstanceName = useAssociatedInstanceName;

			if (m_UseAssociatedInstanceName)
			{
				m_InstanceName = Prepar3D.GetInstanceName();
			}
			else
			{	
				m_InstanceName = null;				
			}
			
			// Recompute the full name because the instance name has changed
			Name = m_Name;
		} 
		
		/** 
		* UseAssociatedInstanceName This function gets whether the variable is using the instance name 
		* of the current movie
		*
		* @return A boolean determining whether the variabnle is using the instance name of the current movie.
		*/
		public function get UseAssociatedInstanceName():Boolean 
		{ 
			return m_UseAssociatedInstanceName; 
		} 
		
		/** 
		* InstanceName This function sets a custom instance name instead of using the movie's instance name.
		*
		* @param instanceName The instance name that should be used to access the scenario variable.
		*/
		public function set InstanceName(instanceName:String):void
		{ 
			m_UseAssociatedInstanceName = false;
			m_InstanceName = instanceName;
			Name = m_Name;
		} 
		
		/** 
		* InstanceName This function gets the instance name that the scenario variable is using.
		*
		* @return The instance name that the scenario variable is using. 
		*/
		public function get InstanceName():String 
		{ 
			return m_InstanceName; 		
		} 
		
		/** 
		* Name This function sets the name that will be used to query for the scenario variable. 
		* This function does extra work to compute the formatted query string and the user only needs to include the
		* scenario variable name and the format will be built up.
		*
		* @param The name of the scenario variable devoid of the instance name of variable prefix.
		*/
		public function set Name(name:String):void
		{ 
			m_Name = name;
			m_ResolvedName = "S:";
			if (InstanceName != null && InstanceName != "")
			{
				m_ResolvedName += InstanceName;
				m_ResolvedName += ":";
			}
			m_ResolvedName += name;
		} 
		
		/** 
		* Name This function gets the fully resolved name that will be used to query for the scenario variable. 
		*
		* @param The name of the scenario variable in a queryable string format that can be used in VarGet and VarSet.
		*/
		public function get Name():String 
		{ 
			return m_ResolvedName; 
		} 
		 
		/** 
		* UnitType This function gets the unit of measurement string that will be used when querying
		* for the scenario variable. 
		*
		* @param The units of measurement as a string.
		*/
		public function get UnitType():String 
		{ 
			return m_UnitType; 
		} 
		
		/** 
		* Value This function gets the scenario variable's value based on its fully resolved name and unit type.
		*
		* @return The scenario variable's value which is of the data type that the unit is in. If the variable was
		* not found, then null is returned.
		*/
		public function get Value():*
		{ 
			return Prepar3D.VarGet(m_ResolvedName, m_UnitType);
		} 
		 
		/** 
		* Value This function sets the scenario variable's value to the value passed in using the prespecified units.
		*
		* @param value The value to set the scenario variable to. The user should know the data type that the scenario
		* variable is expected to be set in and should configure the units accordingly.
		*/
		public function set Value(value:*):void 
		{ 
			Prepar3D.VarSet(m_ResolvedName, m_UnitType, value);
		} 
	}
}
