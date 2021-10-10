import lldb
import struct
import re

def get_target():
	target = lldb.debugger.GetSelectedTarget()
	if not target:
		print("[-] error: no target available. please add a target to lldb.")
		return None
	return target

def findGlobalVariable(target, name):
	sbvar = target.FindGlobalVariables(name, 1).GetValueAtIndex(0)
	if not sbvar.IsValid():
		return None
	return sbvar

def size_of(struct_name):
	res = lldb.SBCommandReturnObject()
	lldb.debugger.GetCommandInterpreter().HandleCommand(f"p sizeof({struct_name})", res)
	if res.GetError():
		# struct is not exists
		return -1
	
	m = re.search(r'\(unsigned long\) \$\d+ = (\d+)\n', res.GetOutput())
	if m:
		return int(m.group(1))
	
	return -1

class ESBValue(object):
	def __init__(self, var_name, var_type=''):
		super().__init__()
		self.target = get_target()
		if var_name == 'classcall':
			self.sb_value = None
		else:
			# find this variable in global context
			self.sb_value = findGlobalVariable(self.target, var_name)
			if not self.sb_value:
				# find this variable in local context
				self.sb_value = lldb.frame.FindVariable(var_name)
				if not self.sb_value.IsValid():
					self.sb_value = None

		if var_type and self.sb_value:
			address = int(self.sb_value.GetValue(), 16)
			self.sb_value = self.target.CreateValueFromExpression('var_name', f'({var_type}){address}')
	
	@classmethod
	def initWithSBValue(cls, sb_value):
		new_esbvalue = cls('classcall')
		new_esbvalue.sb_value = sb_value
		return new_esbvalue
	
	@classmethod
	def initWithAddressType(cls, address, var_type):
		target = get_target()
		new_esbvalue = cls('classcall')
		new_esbvalue.sb_value = target.CreateValueFromExpression('var_name', f'({var_type}){address}')
		return new_esbvalue
	
	def __getattr__(self, name):
		if name == 'sb_value':
			return self.sb_value

		if self.sb_value == None:
			return None
		
		return ESBValue.initWithSBValue(self.GetChildMemberWithName(name))
	
	def GetValue(self):
		if not self.sb_value:
			return None
		return self.sb_value.GetValue()
	
	def GetSummary(self):
		if not self.sb_value:
			return None
		return self.sb_value.GetSummary()
	
	def GetIntValue(self):
		value = self.GetValue()
		if not value:
			return 0
		if value.startswith('0x'):
			return int(value, 16)
		return int(value)
	
	def Dereference(self):
		if not self.sb_value:
			return None
		return ESBValue.initWithSBValue(self.sb_value.Dereference())
	
	def GetBoolValue(self):
		if not self.sb_value:
			return False
		return True if self.GetValue() == 'true' else False
	
	def GetStrValue(self):
		summary = self.GetSummary()
		if summary and 'no value available' not in summary:
			return summary[1:-1] # skip double quote in "data"
		return ''
	
	def GetLoadAddress(self):
		if not self.sb_value:
			return 0
		return self.sb_value.GetLoadAddress()
	
	def GetAddress(self):
		if not self.sb_value:
			return 0
		return self.sb_value.GetAddress()
	
	def GetChildMemberWithName(self, child_name):
		if not self.sb_value:
			return None
		return self.sb_value.GetChildMemberWithName(child_name)
	
	def GetChildAtIndex(self, idx):
		if not self.sb_value:
			return None
		return self.sb_value.GetChildAtIndex(idx)
	
	def IsValid(self):
		if not self.sb_value:
			return False
		return self.sb_value.IsValid()
	
	def __getitem__(self, idx):
		if not self.sb_value:
			return None
		return ESBValue.initWithSBValue(self.GetChildAtIndex(idx))
	
	def CastTo(self, var_type, use_load_addr = False):
		if use_load_addr:
			address = self.GetLoadAddress()
		else:
			address = self.GetIntValue()
		
		new_sb = self.target.CreateValueFromExpression('new_var', f'({var_type}){address}')
		self.sb_value = new_sb
		return self

class Subspace(ESBValue):
	def __init__(self, address):
		super().__init__("classcall")
		self.sb_value = ESBValue.initWithAddressType(address, "JSC::Subspace *").sb_value

	def preciseAllocations(self):
		tmp = []
		size = int(self.m_space.m_preciseAllocations.m_size.GetValue())
		buffer = int(self.m_space.m_preciseAllocations.m_buffer.GetValue(), 16)
		for i in range(0, size, 8):
			ptr = struct.unpack("<Q", lldb.process.ReadMemory(buffer + i, 8, lldb.SBError()))[0]
			tmp.append(ptr)
		return tmp

	def name(self):
		res = lldb.SBCommandReturnObject()
		lldb.debugger.GetCommandInterpreter().HandleCommand(f"p ((JSC::Subspace *){self.GetValue()})->name()", res)
		if res.GetError():
			# struct is not exists
			return ""
		
		m = re.search(r'\(const char \*\) \$\d+ = 0x[0-9a-f]+ \"(.*)\"', res.GetOutput())
		if m:
			return m.group(1)
		
		return ""

class IsoSubspace(Subspace):
	def __init__(self, address):
		super().__init__("classcall")
		self.sb_value = ESBValue.initWithAddressType(address, "JSC::IsoSubspace *").sb_value

class CompleteSubspace(Subspace):
	def __init__(self, address):
		super().__init__("classcall")
		self.sb_value = ESBValue.initWithAddressType(address, "JSC::CompleteSubspace *").sb_value

	def contain(self, address):
		size = 503
		print(self.m_allocatorForSizeStep.GetLoadAddress())
		buffer = int(self.m_allocatorForSizeStep.GetLoadAddress())

		for i in range(0, size, 8):
			ptr = struct.unpack("<Q", lldb.process.ReadMemory(buffer + i, 8, lldb.SBError()))[0]
			if ptr == 0: continue
			local = LocalAllocator(ptr)
			if local.contain(address):
				return i
			
		return -1

class LocalAllocator(ESBValue):
	def __init__(self, address):
		super().__init__("classcall")
		self.sb_value = ESBValue.initWithAddressType(address, "JSC::LocalAllocator *").sb_value
	def contain(self, address):
		end = int(self.m_freeList.m_payloadEnd.GetValue(), 16)
		orig_size = int(self.m_freeList.m_originalSize.GetValue(), 16)
		return end - orig_size <= address < end

class PreciseAllocation(ESBValue):
	def __init__(self, address):
		super().__init__("classcall")
		self.sb_value = ESBValue.initWithAddressType(address, "JSC::PreciseAllocation *").sb_value

	def cell(self):
		res = lldb.SBCommandReturnObject()
		lldb.debugger.GetCommandInterpreter().HandleCommand(f"p ((JSC::PreciseAllocation *){self.GetValue()})->cell()", res)
		if res.GetError():
			# struct is not exists
			return ""
		
		m = re.search(r'\(JSC::HeapCell \*\) \$\d+ = (0x[0-9a-f]+)', res.GetOutput())
		if m:
			return m.group(1)
		
		return ""

	def contains(self, ptr):
		res = lldb.SBCommandReturnObject()
		lldb.debugger.GetCommandInterpreter().HandleCommand(f"p ((JSC::PreciseAllocation *){self.GetValue()})->contains((void *){ptr})", res)
		if res.GetError():
			# struct is not exists
			return False
		m = re.search(r'\(bool\) \$\d+ = (true|false)', res.GetOutput())
		if m:
			return True if m.group(1) == "true" else False
		
		return False


class VirtualMachine(ESBValue):
	def __init__(self, address):
		super().__init__("classcall")
		self.sb_value = ESBValue.initWithAddressType(address, "JSC::VM *").sb_value

	def getCompleteSpace(self):
		properties = [
			"primitiveGigacageAuxiliarySpace",
			"jsValueGigacageAuxiliarySpace",
			"immutableButterflyJSValueGigacageAuxiliarySpace",
			"cellSpace",
			"variableSizedCellSpace",
			"destructibleObjectSpace",
		]

		result = []
		for p in properties:
			result.append(CompleteSubspace(getattr(self, p).GetLoadAddress()))
		return result

	def getIsoSubspace(self):
		properties = [
			"arraySpace",
			"bigIntSpace",
			"calleeSpace",
			"clonedArgumentsSpace",
			"customGetterSetterSpace",
			"dateInstanceSpace",
			"domAttributeGetterSetterSpace",
			"exceptionSpace",
			"executableToCodeBlockEdgeSpace",
			"functionSpace",
			"getterSetterSpace",
			"globalLexicalEnvironmentSpace",
			"internalFunctionSpace",
			"jsProxySpace",
			"nativeExecutableSpace",
			"numberObjectSpace",
			"plainObjectSpace",
			"promiseSpace",
			"propertyNameEnumeratorSpace",
			"propertyTableSpace",
			"regExpSpace",
			"regExpObjectSpace",
			"ropeStringSpace",
			"scopedArgumentsSpace",
			"sparseArrayValueMapSpace",
			"stringSpace",
			"stringObjectSpace",
			"structureChainSpace",
			"structureRareDataSpace",
			"structureSpace",
			"brandedStructureSpace",
			"symbolTableSpace",
			# "codeBlockSpace",
			# "functionExecutableSpace",
			# "programExecutableSpace",
			# "unlinkedFunctionExecutableSpace"
		]

		result = []
		for p in properties:
			result.append(IsoSubspace(getattr(self, p).GetLoadAddress()))
		return result

output = """
[ MY DEBUG ] -----> VM's address = 0x7f9c55013000
[ MY DEBUG ] -----> VM's address = 0x7f8175713000
[ MY DEBUG ] -----> VM's address = 0x7f94b8e13000
"""

def locate(addr):
	regex = r"0x[0-9a-f]+"
	matches = re.finditer(regex, output)
	found = False
	for match in matches:
		if found is True: break
		vm = VirtualMachine(int(match.group(0), 16))
		spaces = vm.getCompleteSpace()
		for sp in spaces:
			if found is True: break
			print("[+] Trying: %s --- %s" % (vm.GetValue(), sp.name()))
			result = sp.contain(addr)
			if result != -1:
				print("[=>] Found:\n\tVM:%s\n\tSpace:%s\n\tIndex:%d" % (vm.GetValue(), sp.name(), result))
				found = True
				break

def locateInVM(vm_addr, addr):
	vm = VirtualMachine(vm_addr)
	spaces = vm.getCompleteSpace()
	found = False
	for sp in spaces:
		if found is True: break
		print("[+] Trying: %s --- %s" % (vm.GetValue(), sp.name()))
		result = sp.contain(addr)
		if result != -1:
			print("[=>] Found:\n\tVM:%s\n\tSpace:%s\n\tIndex:%d" % (vm.GetValue(), sp.name(), result))
			found = True
			break